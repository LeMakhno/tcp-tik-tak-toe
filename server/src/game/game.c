#include "game.h"
#include "../utils/generic_utils.h"
#include "../utils/string_utils.h"

game_t *create_game()
{
    game_t *game = calloc_w(1, sizeof(game_t));
    game->id = random_id(20);
    game->status = GAME_ST_NEW;
    
    for (int i = 0; i < GAME_DIMENSION * GAME_DIMENSION; i++) {
        game->board[i] = ' ';
    }
    
    return game;
}

void free_game(game_t *game)
{
    if (!game) {
        return;
    }
    free(game->id);
    free(game);
}

void handle_game_quit(session_t *session, game_t *game)
{
    if (session->status == SESSION_ST_HOST) {
        game->initiator = NULL;
    } else {
        game->guest = NULL;
    }
    /* Both disconnected */
    if (!game->initiator && !game->guest) {
        game->status = GAME_ST_REMOVABLE;
        return;
    }
    /* One disconnected */
    game->status = GAME_ST_ABANDONED;
    session_t *oponent = session->status == SESSION_ST_HOST
        ? game->guest
        : game->initiator;

    enqueue_message(
        oponent->connection,
        new_fstring(OUT_BUFSIZE, "message@ok@%s left;", session->nickname)
    );
}
