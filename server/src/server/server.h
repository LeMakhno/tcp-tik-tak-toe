#ifndef SERVER
#define SERVER 1

#include "../include/common.h"
#include "../clients/connset.h"

typedef struct server
{
    int server_fd;
    volatile bool running;
} server_t;

server_t *get_server();
void server_init(char *port, int backlog);

#endif
