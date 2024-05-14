#include "command_handlers.h"

void handle_leave(connection_t *connection, string_arr_t arguments)
{
    session_t *session = find_session(connection->session_id);
    if (session->status == SESSION_ST_IDLE) {
        enqueue_message(
            connection,
            new_string("message@err@Not in game;")
        );
        return;
    }

    game_t *game = find_game(session->game_id);
    handle_game_quit(session, game);
    if (game->status == GAME_ST_REMOVABLE) {
        putlog("Removing game %s", game->id);
        remove_game(game->id);
        free_game(game);
    }

    session->game_id = NULL;
    session->symbol = '\0';
    session->status = SESSION_ST_IDLE;

    enqueue_message(
        connection,
        new_string("message@ok@Successfully left the game;")
    );
}
