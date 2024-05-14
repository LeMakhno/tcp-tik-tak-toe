#include "command_handlers.h"
#include "../game/board_logic.h"
#include "common_logic.h"

static bool can_act(session_t *session, game_t *game)
{
    bool can_act_as_host = session->status == SESSION_ST_HOST
        && game->status == GAME_ST_HOST_TURN;
    bool can_act_as_guest = session->status == SESSION_ST_GUEST
        && game->status == GAME_ST_GUEST_TURN;
    
    return can_act_as_host || can_act_as_guest;
}

static int do_act(char *board, char symbol, char oponent_symbol, int idx)
{
    board[idx] = symbol;
    if (contains_victorious_combination(board, symbol)) {
        return ACT_RESULT_WIN;
    }

    bool current_player_victory_possible = is_victory_possible(
        board,
        symbol
    );
    bool oponent_vistory_possible = is_victory_possible(
        board,
        oponent_symbol
    );

    /*
    if (current_player_victory_possible && !oponent_vistory_possible) {
        return ACT_RESULT_WIN;
    }
    */
    if (!current_player_victory_possible && !oponent_vistory_possible) {
        return ACT_RESULT_DRAW;
    }
    
    return ACT_RESULT_CONTINUE;
}

static void on_win(game_t *game, session_t *session, session_t *oponent)
{
    enqueue_message(
        session->connection,
        new_string("message@ok@You won;")
    );
    enqueue_message(
        oponent->connection,
        new_fstring(OUT_BUFSIZE, "message@ok@%s won;", session->nickname)
    );
    game->status = GAME_ST_OVER;
}

static void on_draw(game_t *game, session_t *session, session_t *oponent)
{
    char *draw_response = "message@ok@The game was a draw;";
    enqueue_message(
        session->connection,
        new_string(draw_response)
    );
    enqueue_message(
        oponent->connection,
        new_string(draw_response)
    );
    game->status = GAME_ST_OVER;
}

static void on_continue(game_t *game, session_t *session, session_t *oponent)
{
    game->status = game->status == GAME_ST_HOST_TURN
        ? GAME_ST_GUEST_TURN
        : GAME_ST_HOST_TURN;
    enqueue_message(
        session->connection,
        new_fstring(OUT_BUFSIZE, "message@ok@%s's turn;", oponent->nickname)
    );
    enqueue_message(
        oponent->connection,
        new_fstring(OUT_BUFSIZE, "message@ok@%s's turn;", oponent->nickname)
    );
}

void handle_put(connection_t *connection, string_arr_t arguments)
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

    bool is_game_playable = game->status == GAME_ST_GUEST_TURN
        || game->status == GAME_ST_HOST_TURN;
    
    if (!is_game_playable) {
        enqueue_message(
            connection,
            new_string("message@err@The game is not playable;")
        );
        return;
    }
    if (!can_act(session, game)) {
        enqueue_message(
            connection,
            new_string("message@err@Not your turn;")
        );
        return;
    }

    int cell = atoi(arguments.strings[1]);
    int board_idx = cell - 1;

    if (cell < 1 || cell > GAME_DIMENSION * GAME_DIMENSION) {
        enqueue_message(
            connection,
            new_string("message@err@Invalid cell value;")
        );
        return;
    }
    if (game->board[board_idx] != ' ') {
        enqueue_message(
            connection,
            new_string("message@err@The cell is not available;")
        );
        return;
    }

    session_t *oponent = session->status == SESSION_ST_HOST
        ? game->guest
        : game->initiator;

    int act_result = do_act(
        game->board,
        session->symbol,
        oponent->symbol,
        board_idx
    );

    switch (act_result) {
        case ACT_RESULT_WIN:
            on_win(game, session, oponent);
            break;
        case ACT_RESULT_DRAW:
            on_draw(game, session, oponent);
            break;
        default:
            on_continue(game, session, oponent);
            break;
    }
    send_board(game, session, oponent);
}
