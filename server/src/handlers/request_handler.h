#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER 1

#include "../clients/connection.h"
#include "../utils/string_utils.h"

void request_handler(connection_t *connection, string_arr_t arguments);

#endif
