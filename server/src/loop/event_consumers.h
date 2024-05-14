#ifndef EVENT_CONSUMERS
#define EVENT_CONSUMERS 1

#include "../clients/connection.h"

typedef void (*event_consumer_t)(connection_t*);

void on_hangup(connection_t *connection);
void on_pollin(connection_t *connection);
void on_pollout(connection_t *connection);
void on_pollerr(connection_t *connection);
void on_read_available(connection_t *connection);
void on_write_available(connection_t *connection);
void on_request_ready(connection_t *connection);
void on_connection(connection_t *connection);
void on_disconnection(connection_t *connection);

#endif
