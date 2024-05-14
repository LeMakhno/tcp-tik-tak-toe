#include "command_handlers.h"
#include "common_logic.h"

void handle_join(connection_t *connection, string_arr_t arguments)
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

    char *game_id = arguments.strings[1];

    game_t *game = find_game(game_id);
    if (!game) {
        enqueue_message(
            connection,
            new_string("message@err@Game not found;")
        );
        return;
    }
    if (game->status != GAME_ST_NEW) {
        enqueue_message(
            connection,
            new_string("message@err@Can't join;")
        );
        return;
    }

    game->guest = session;
    game->status = random_bool() ? GAME_ST_HOST_TURN : GAME_ST_GUEST_TURN;
    session->game_id = game->id;
    session->status = SESSION_ST_GUEST;
    session->symbol = game->initiator->symbol == 'x' ? 'o' : 'x';

    enqueue_message(connection, new_fstring(
        OUT_BUFSIZE,
        "message@ok@Successfully joined, your symbol is %c;",
        session->symbol
    ));
    enqueue_message(game->initiator->connection, new_fstring(
        OUT_BUFSIZE,
        "message@ok@%s joined;",
        session->nickname
    ));

    session_t *starts_first = game->status == GAME_ST_HOST_TURN
        ? game->initiator
        : game->guest;
    
    char *start_message = new_fstring(
        OUT_BUFSIZE,
        "message@ok@%s starts;",
        starts_first->nickname
    );
    enqueue_message(connection, new_string(start_message));
    enqueue_message(game->initiator->connection, new_string(start_message));
    free(start_message);

    send_board(game, session, game->initiator);
}
