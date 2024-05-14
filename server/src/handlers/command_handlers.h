#ifndef COMMAND_HANDLER
#define COMMAND_HANDLER 1

#include "../clients/connection.h"
#include "../utils/string_utils.h"
#include "../storage/storage.h"
#include "../utils/generic_utils.h"
#include "../utils/logger.h"

#define NOT_REGISTERED_RESPONSE "err,Register a nickname first;"
#define NOT_IN_GAME_RESPONSE "err,Not in game;"
#define ALREADY_IN_GAME_RESPONSE "err,Already in game;"
#define GAME_NOT_FOUND_RESPONSE "err,Game not found;"

typedef void (*command_handler_t)(connection_t *, string_arr_t);

typedef struct command {
    char *name;
    int arglen;
    command_handler_t handler;
} command_t;

void handle_reg(connection_t *connection, string_arr_t arguments);
void handle_init(connection_t *connection, string_arr_t arguments);
void handle_join(connection_t *connection, string_arr_t arguments);
void handle_leave(connection_t *connection, string_arr_t arguments);
void handle_put(connection_t *connection, string_arr_t arguments);
void handle_status(connection_t *connection, string_arr_t arguments);

#endif
