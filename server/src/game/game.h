#ifndef GAME
#define GAME 1

#include "../session/session.h"

#define GAME_ST_NEW 1
#define GAME_ST_STARTED 2
#define GAME_ST_ABANDONED 3
#define GAME_ST_REMOVABLE 4
#define GAME_ST_HOST_TURN 5
#define GAME_ST_GUEST_TURN 6
#define GAME_ST_OVER 7

#define ACT_RESULT_WIN 1
#define ACT_RESULT_CONTINUE 2
#define ACT_RESULT_DRAW 3

#define GAME_DIMENSION 3

typedef struct game {
    char *id;
    session_t *initiator;
    session_t *guest;
    int status;
    char board[GAME_DIMENSION * GAME_DIMENSION + 1];
} game_t;

game_t *create_game();
void free_game(game_t *game);
void handle_game_quit(session_t *session, game_t *game);

#endif
