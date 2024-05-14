#include "common_logic.h"
#include "../game/board_logic.h"
#include "../utils/generic_utils.h"

void send_board(game_t *game, session_t *session, session_t *oponent)
{
    char *board = printable_board(game->board);
    enqueue_message(
        session->connection,
        new_fstring(OUT_BUFSIZE, "board@ok@%s;", board)
    );
    enqueue_message(
        oponent->connection,
        new_fstring(OUT_BUFSIZE, "board@ok@%s;", board)
    );
    free(board);
}
