#ifndef _BOARD_LOGIC
#define _BOARD_LOGIC 1

#include "game.h"

bool contains_victorious_combination(char *board, char symbol);
bool is_victory_possible(char *board, char symbol);
char *printable_board(char *board);

#endif
