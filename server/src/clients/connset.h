#ifndef CLIENTSET
#define CLIENTSET 1

#include "connection.h"
#include "../include/common.h"
#include "../loop/event_consumers.h"

#define MAX_CLIENTS 5000

typedef struct connset
{
    volatile int size;
    volatile int set_idx;
    connection_t *connection_arr[MAX_CLIENTS];
    pollfd_t pollfd_arr[MAX_CLIENTS];
} connset_t;

void connset_init();
connset_t *get_connset();
void connset_free();
bool connset_add(connection_t *connection, pollfd_t pollfd);
void connset_cleanup(event_consumer_t disconnection_callback);
bool connset_isfull();

#endif
