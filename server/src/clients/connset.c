#include "connset.h"
#include "../utils/logger.h"
#include "../utils/generic_utils.h"

static connset_t *connset;

void connset_init()
{   
    connset = calloc_w(1, sizeof(connset_t));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        connset->pollfd_arr[i].fd = FD_POLLIGNORE;
    }
}

connset_t *get_connset()
{
    return connset;
}

void connset_free()
{
    for (size_t i = 0; i < MAX_CLIENTS; i++) {
        connection_t *connection = connset->connection_arr[i];
        pollfd_t *pollfd = &connset->pollfd_arr[i];
        if (connection) {
            close_w(pollfd->fd, true);
            free(connection);
        }
    }
    free(connset);
}

bool connset_add(connection_t *connection, pollfd_t pollfd) {
    if (connset_isfull()) {
        return false;
    }

    int set_idx = connset->set_idx;

    connset->connection_arr[set_idx] = connection;
    connset->pollfd_arr[set_idx] = pollfd;
    connset->size++;

    for (int i = set_idx + 1; i < MAX_CLIENTS; i++) {
        connection_t *connection = connset->connection_arr[i];
        if (!connection) {
            connset->set_idx = i;
            break;
        }
    }

    return true;
}

void connset_cleanup(event_consumer_t disconnection_callback)
{
    int set_idx = -1;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        connection_t *connection = connset->connection_arr[i];
        pollfd_t *pollfd = &connset->pollfd_arr[i];

        if (!connection && set_idx < 0) {
            set_idx = i;
        }

        if (!connection || connection->conn_state != ST_CONN_DEAD) {
            continue;
        }

        if (set_idx < 0) {
            set_idx = i;
        }
        
        connset->connection_arr[i] = NULL;
        
        disconnection_callback(connection);
        free(connection);

        close_w(pollfd->fd, false);
        pollfd->fd = FD_POLLIGNORE;
        pollfd->events = 0;
        pollfd->revents = 0;

        connset->size--;
    }

    if (set_idx >= 0) {
        connset->set_idx = set_idx;
    }
}

bool connset_isfull()
{
    return connset->size == MAX_CLIENTS;
}
