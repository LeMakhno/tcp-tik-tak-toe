#include "../clients/connset.h"
#include "../utils/generic_utils.h"
#include "../utils/net_utils.h"
#include "../include/common.h"
#include "../utils/logger.h"
#include "event_consumers.h"
#include "loop.h"
#include "../server/server.h"
#include "../utils/collections/linked_queue.h"

static bool is_pollable(connection_t *connection)
{
    return connection != NULL
        && connection->conn_state != ST_CONN_DEAD;
}

static bool is_processable(connection_t *connection)
{
    return connection != NULL
        && connection->conn_state != ST_CONN_DEAD;
}

static void accept_connections()
{
    pollfd_t server_conn_event = {
        .fd = get_server()->server_fd,
        .events = POLLIN,
        .revents = 0
    };

    int available = poll(&server_conn_event, 1, 1);
    if (available == ERR) {
        putlog(
            "Error %d while polling for new connections: %s",
            errno,
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    if (!available) {
        return;
    }
    
    /* Accept all pending connections */
    while (true) {
        connection_t conn_data_storage;
        mclear(&conn_data_storage, sizeof(connection_t));
        conn_data_storage.addrlen = sizeof(conn_data_storage.sockaddr);

        int new_fd = accept(
            get_server()->server_fd,
            &conn_data_storage.sockaddr,
            &conn_data_storage.addrlen
        );

        if (new_fd == ERR) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                putlog("Error while accepting connection: %s", strerror(errno));
                exit(EXIT_FAILURE);
            }
            /* EAGAIN, all connections accepted */
            break;
        }

        sockaddr_to_string(
            conn_data_storage.addr_printable,
            &conn_data_storage.sockaddr
        );

        if (connset_isfull()) {
            putlog("Can't handle more clients, disconnecting: %s",
                conn_data_storage.addr_printable);
            close_w(new_fd, false);
            continue;
        }

        connection_t *connection = calloc_w(1, sizeof(connection_t));
        memcpy(connection, &conn_data_storage, sizeof(connection_t));
        connection->sock_fd = new_fd;
        set_init_connection_state(connection);
        
        connset_add(connection, (pollfd_t) {
            .fd = new_fd,
            .events = POLLIN | POLLOUT,
            .revents = 0
        });

        on_connection(connection);
    }
}

static void poll_clientset()
{
    int available = poll(get_connset()->pollfd_arr, MAX_CLIENTS, 10);
    if (available == ERR) {
        putlog(
            "Error %d while polling connections: %s",
            errno,
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    if (!available) {
        return;
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {

        connection_t *connection = get_connset()->connection_arr[i];
        pollfd_t *pollfd = &get_connset()->pollfd_arr[i];

        if (!is_pollable(connection)) {
            continue;
        }

        if (pollfd->revents & POLLOUT) {
            on_pollout(connection);
        }
        if (pollfd->revents & POLLIN) {
            on_pollin(connection);
        }
        if (pollfd->revents & POLLERR) {
            on_pollerr(connection);
        }

        pollfd->revents = 0;
    }
}

static void perform_io()
{
    for (int i = 0; i < MAX_CLIENTS; i++) {

        connection_t *connection = get_connset()->connection_arr[i];
        if (!is_processable(connection)) {
            continue;
        }

        if (connection->in_state == ST_IO_ACTIVE) {
            on_read_available(connection);
        }
        if (connection->out_state == ST_IO_ACTIVE) {
            on_write_available(connection);
        }
    }
}

static void handle_requests()
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        
        connection_t *connection = get_connset()->connection_arr[i];
        if (!connection) {
            continue;
        }

        if (connection->in_state == ST_IO_REQUEST_READY) {
            on_request_ready(connection);
        }
    }
}

static void cleanup()
{
    int before = get_connset()->size;
    connset_cleanup(on_disconnection);
    int diff = before - get_connset()->size;
    if (diff) {
        putlog("Removed %d connections", diff);
        putlog("Connections: %d", get_connset()->size);
    }
}

void run_loop()
{
    while (get_server()->running) {
        accept_connections();
        poll_clientset();
        perform_io();
        handle_requests();
        cleanup();
    }
}
