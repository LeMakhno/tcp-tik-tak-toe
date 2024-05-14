#include "../../include/common.h"
#include "protocol.h"
#include "../../utils/generic_utils.h"
#include "../../utils/string_utils.h"

char *consume_message(connection_t *connection)
{
    char *end_of_request = strchr(connection->__in_buf, END_OF_REQUEST_CHAR);
    int request_len = end_of_request - connection->__in_buf;

    char *message = calloc_w(request_len + 1, sizeof(char));
    memcpy(message, connection->__in_buf, request_len);
    trim(message);

    int remaining_data_len = (connection->__in_buf_current - (void *) end_of_request) - 1;
    /* Clear buffer if nothing is left */
    if (!remaining_data_len) {
        mclear(connection->__in_buf, request_len + 1);
        connection->__in_buf_current = connection->__in_buf;
        if (connection->conn_state != ST_CONN_DEAD) {
            connection->in_state = ST_IO_IDLE;
        }
        return message;
    }

    /* Rebase remaining data */
    memmove(connection->__in_buf, end_of_request + 1, remaining_data_len);
    mclear(connection->__in_buf + remaining_data_len, IN_BUFSIZE - remaining_data_len);
    connection->__in_buf_current = connection->__in_buf + remaining_data_len;

    /* Check if another request available */
    if (strchr(connection->__in_buf, END_OF_REQUEST_CHAR)) {
        connection->in_state = ST_IO_REQUEST_READY;
    } else {
        if (connection->conn_state != ST_CONN_DEAD) {
            connection->in_state = ST_IO_IDLE;
        }
    }

    return message;
}

bool contains_message(char *buffer)
{
    char *position = strchr(buffer, END_OF_REQUEST_CHAR);
    return position != NULL;
}
