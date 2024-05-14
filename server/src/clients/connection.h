#ifndef CLIENTDATA
#define CLIENTDATA 1

#include "../include/common.h"
#include "../utils/collections/linked_queue.h"

#define ADDR_PRINTABLE_BUFSIZE 128

#define IN_BUFSIZE 256
#define OUT_BUFSIZE 256

#define ST_CONN_ALIVE 1
#define ST_CONN_DEAD -1
#define ST_CONN_REFUSED -2

#define ST_IO_REQUEST_READY 1
#define ST_IO_ACTIVE 2
#define ST_IO_IDLE 3
#define ST_IO_FULL 4

#define FD_POLLIGNORE -1

typedef struct connection
{
    int sock_fd;
    atomic_int conn_state;
    int in_state;
    int out_state;
    sockaddr_t sockaddr;
    socklen_t addrlen;
    char addr_printable[ADDR_PRINTABLE_BUFSIZE];
    char __in_buf[IN_BUFSIZE + 1];
    void *__in_buf_current;
    char __out_buf[OUT_BUFSIZE + 1];
    void *__out_buf_current;
    linked_queue_t *__out_queue;
    char *session_id;
} connection_t;

void enqueue_message(connection_t *connection, char *message);
void set_init_connection_state(connection_t *connection);
bool contains_writable_data(connection_t *connection);
long bytes_in_input_buffer(connection_t *connection);

#endif
