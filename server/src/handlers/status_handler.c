#include "command_handlers.h"

static char *nickname_info(session_t *session)
{
    return session->nickname
        ? new_fstring(128, "Nickname: %s", session->nickname)
        : new_string("No nickname");
}

static char *session_state_info(session_t *session)
{
    switch (session->status) {
        case SESSION_ST_IDLE:
            return new_string("not in game");
        case SESSION_ST_HOST:
            return new_string("hosting a game");
        case SESSION_ST_GUEST:
            return new_string("in game");
        default:
            return new_string("unknown session status");
    }
}

void handle_status(connection_t *connection, string_arr_t arguments)
{
    session_t *session = find_session(connection->session_id);

    char *nickname_inf = nickname_info(session);
    char *session_inf = session_state_info(session);

    enqueue_message(connection, new_fstring(
        OUT_BUFSIZE,
        "message@ok@%s, %s;", nickname_inf, session_inf
    ));

    free(nickname_inf);
    free(session_inf);
}
