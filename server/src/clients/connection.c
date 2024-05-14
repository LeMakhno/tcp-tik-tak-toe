#include "connection.h"
#include "../utils/collections/linked_queue.h"
#include "../utils/logger.h"

void enqueue_message(connection_t *connection, char *message)
{
    putlog("Out: %s", message);
    queue_put(connection->__out_queue, message);
}

void set_init_connection_state(connection_t *connection)
{
    connection->conn_state = ST_CONN_ALIVE;
    connection->in_state = ST_IO_IDLE;
    connection->__in_buf_current = connection->__in_buf;
    connection->out_state = ST_IO_IDLE;
    connection->__out_buf_current = connection->__out_buf;
    queue_init(&connection->__out_queue);
}

bool contains_writable_data(connection_t *connection)
{
    char current_value = *((char *) connection->__out_buf_current);
    return current_value != 0;
}

long bytes_in_input_buffer(connection_t *connection)
{
    return connection->__in_buf_current - (void *) connection->__in_buf;
}
