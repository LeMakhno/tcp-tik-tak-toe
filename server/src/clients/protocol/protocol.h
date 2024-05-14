#ifndef MESSAGING_PROTOCOL
#define MESSAGING_PROTOCOL 1

#define END_OF_REQUEST_CHAR ';'
#define REQUEST_ELEMENT_SEPARATOR '@'

#include "../connection.h"

char *consume_message(connection_t *connection);
bool contains_message(char *buffer);

#endif
