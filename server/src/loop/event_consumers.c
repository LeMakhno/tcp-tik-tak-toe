#include "event_consumers.h"
#include "../utils/net_utils.h"
#include "../utils/generic_utils.h"
#include "../utils/string_utils.h"
#include "../utils/logger.h"
#include "../handlers/request_handler.h"
#include "../storage/storage.h"
#include "../clients/protocol/protocol.h"

void on_hangup(connection_t *connection)
{
    putlog("Hangup: %s", connection->addr_printable);
    connection->conn_state = ST_CONN_DEAD;
}

void on_pollin(connection_t *connection)
{
    bool hangup = test_hangup(connection->sock_fd);
    if (hangup) {
        on_hangup(connection);
        return;
    }

    connection->in_state = ST_IO_ACTIVE;
}

void on_pollout(connection_t *connection)
{
    if (connection->out_state != ST_IO_IDLE) {
        return;
    }

    if (contains_writable_data(connection)) {
        connection->out_state = ST_IO_ACTIVE;
        return;
    }

    if (connection->__out_queue->size) {
        char *message = queue_poll(connection->__out_queue);
        memcpy(connection->__out_buf, message, strlen(message));
        connection->out_state = ST_IO_ACTIVE;
        free(message);
    }
}

void on_pollerr(connection_t *connection)
{
    int sockerr = 0;
    socklen_t errlen = sizeof(sockerr);
    int geterr_result = getsockopt(
        connection->sock_fd,
        SOL_SOCKET,
        SO_ERROR,
        &sockerr,
        &errlen
    );
    if (sockerr && geterr_result != ERR) {
        putlog(
            "%s socket error: %d, %s",
            connection->addr_printable,
            sockerr,
            strerror(sockerr)
        );
    }
}

void on_read_available(connection_t *connection)
{
    long bytes_in_buf = bytes_in_input_buffer(connection);
    long remaining_bufsize = IN_BUFSIZE - bytes_in_buf;

    if (!remaining_bufsize || remaining_bufsize < 0) {
        putlog(
            "WARNING - %s input buffer is full, end of request not received",
            connection->addr_printable
        );
        connection->conn_state = ST_CONN_DEAD;
        return;
    }

    readall_nio(connection);

    if (contains_message(connection->__in_buf)) {
        connection->in_state = ST_IO_REQUEST_READY;
    }
}

void on_write_available(connection_t *connection)
{
    sendall_nio(connection);

    if (!contains_writable_data(connection)) {
        connection->out_state = ST_IO_IDLE;
        connection->__out_buf_current = connection->__out_buf;
        mclear(connection->__out_buf, OUT_BUFSIZE);
    }
}

void on_request_ready(connection_t *connection)
{
    char *request = consume_message(connection);
    putlog("Handling: %s", request);

    string_arr_t arguments = split_string(request, REQUEST_ELEMENT_SEPARATOR);
    request_handler(connection, arguments);

    free_string_arr(arguments);
    free(request);
}

void on_connection(connection_t *connection)
{
    putlog("Connected: %s", connection->addr_printable);
    session_t *session = create_session();
    session->connection = connection;
    connection->session_id = session->id;
    add_session(session);
}

void on_disconnection(connection_t *connection)
{
    putlog("Disconnecting: %s", connection->addr_printable);
    free_queue(connection->__out_queue, true);
    session_t *session = remove_session(connection->session_id);
    if (!session) {
        return;
    }
    remove_nickname(session->nickname);
    if (session->status == SESSION_ST_IDLE) {
        free_session(session);
        return;
    }
    game_t *game = find_game(session->game_id);
    handle_game_quit(session, game);
    if (game->status == GAME_ST_REMOVABLE) {
        remove_game(session->game_id);
        free_game(game);
    }
    free_session(session);
}
