#include "request_handler.h"
#include "../utils/string_utils.h"
#include "../utils/generic_utils.h"
#include "../utils/logger.h"
#include "../session/session.h"
#include "../storage/storage.h"
#include "command_handlers.h"

static const command_t REG = {
    .name = "reg",
    .arglen = 2,
    .handler = handle_reg
};
static const command_t INIT = {
    .name = "init",
    .arglen = 1,
    .handler = handle_init
};
static const command_t JOIN = {
    .name = "join",
    .arglen = 2,
    .handler = handle_join
};
static const command_t LEAVE = {
    .name = "leave",
    .arglen = 1,
    .handler = handle_leave
};
static const command_t PUT = {
    .name = "put",
    .arglen = 2,
    .handler = handle_put
};
static const command_t STATUS = {
    .name = "status",
    .arglen = 1,
    .handler = handle_status
};

void request_handler(connection_t *connection, string_arr_t arguments)
{
    if (!arguments.elements_num) {
        enqueue_message(
            connection,
            new_string("message@err@Invalid request;")
        );
        return;
    }

    char *command_name = arguments.strings[0];
    const command_t *command = NULL;

    if (streq(command_name, REG.name)) {
        command = &REG;
    }
    else if (streq(command_name, JOIN.name)) {
        command = &JOIN;
    }
    else if (streq(command_name, LEAVE.name)) {
        command = &LEAVE;
    }
    else if (streq(command_name, PUT.name)) {
        command = &PUT;
    }
    else if (streq(command_name, STATUS.name)) {
        command = &STATUS;
    }
    else if (streq(command_name, INIT.name)) {
        command = &INIT;
    }

    if (!command) {
        enqueue_message(
            connection,
            new_string("message@err@Unknown command;")
        );
        return;
    }

    if (command->arglen == arguments.elements_num) {
        command->handler(connection, arguments);
    } else {
        enqueue_message(
            connection,
            new_string("message@err@Invalid command arguments;")
        );
    }
}
