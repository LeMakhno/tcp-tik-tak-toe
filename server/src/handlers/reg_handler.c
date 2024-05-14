#include "command_handlers.h"

void handle_reg(connection_t *connection, string_arr_t arguments)
{
    session_t *session = find_session(connection->session_id);
    if (session->status != SESSION_ST_IDLE) {
        enqueue_message(
            connection,
            new_string("message@err@Already in game;")
        );
        return;
    }
    if (session->nickname) {
        enqueue_message(
            connection,
            new_string("message@err@Nickname already acquired;")
        );
        return;
    }

    bool is_taken = is_nickname_taken(arguments.strings[1]);
    if (is_taken) {
        enqueue_message(
            connection,
            new_string("message@err@This nickname is already taken;")
        );
        return;
    }

    char *nickname = new_string(arguments.strings[1]);

    session->nickname = nickname;
    add_nickname(nickname);

    enqueue_message(
        connection,
        new_string("message@ok@Nickname successfully registered;")
    );
}
