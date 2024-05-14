#include "command_handlers.h"

void handle_init(connection_t *connection, string_arr_t arguments)
{
    session_t *session = find_session(connection->session_id);
    if (!session->nickname) {
        enqueue_message(
            connection,
            new_string("message@err@Register a nickname first;")
        );
        return;
    }
    if (session->status != SESSION_ST_IDLE) {
        enqueue_message(
            connection,
            new_string("message@err@Already in game;")
        );
        return;
    }

    game_t *new_game = create_game();
    new_game->initiator = session;
    add_game(new_game);

    session->game_id = new_game->id;
    session->status = SESSION_ST_HOST;
    session->symbol = random_symbol("xo");

    enqueue_message(connection, new_fstring(
        OUT_BUFSIZE,
        "message@ok@New game id: %s, symbol %c;",
        new_game->id,
        session->symbol
    ));
}
