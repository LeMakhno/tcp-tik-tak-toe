#ifndef STORAGE
#define STORAGE 1

#include "../game/game.h"
#include "../session/session.h"

void storage_init();

/* Game related */
void add_game(game_t *game);
game_t *find_game(char *id);
game_t *remove_game(char *id);

/* Nickname related */
void add_nickname(char *nickname);
bool is_nickname_taken(char *nickname);
void remove_nickname(char *nickname);

/* Session related */
void add_session(session_t *session);
session_t *find_session(char *id);
session_t *remove_session(char *id);

#endif
