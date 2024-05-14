#include "board_logic.h"
#include "../utils/generic_utils.h"

static const int PRINTABLE_BOARD_LEN = (GAME_DIMENSION + GAME_DIMENSION)
    * (GAME_DIMENSION + GAME_DIMENSION - 1);

static bool row_consists_of_symbols(char *board, int row, char *symbols)
{
    for (int col = 0; col < GAME_DIMENSION; col++) {
        if (!strchr(symbols, board[GAME_DIMENSION * row + col])) {
            return false;
        }
    }
    return true;
}

static bool col_consists_of_symbols(char *board, int col, char *symbols)
{
    for (int row = 0; row < GAME_DIMENSION; row++) {
        if (!strchr(symbols, board[GAME_DIMENSION * row + col])) {
            return false;
        }
    }
    return true;
}

static bool any_diagonal_consists_of_symbols(char *board, char *symbols)
{
    int idx = -1;
    bool match = true;

    /* Top left to bottom right */
    for (int i = 0; i < GAME_DIMENSION; i++) {
        idx = i * GAME_DIMENSION + i;
        if (!strchr(symbols, board[idx])) {
            match = false;
            break;
        }
    }

    if (match) {
        return true;
    }

    match = true;

    /* Top right to bottom left */
    for (int i = 0; i < GAME_DIMENSION; i++) {
        idx = i * GAME_DIMENSION + GAME_DIMENSION - i - 1;
        if (!strchr(symbols, board[idx])) {
            match = false;
            break;
        }
    }

    return match;
}

bool contains_victorious_combination(char *board, char symbol)
{
    char symbols[2] = {symbol, '\0'};
    for (int i = 0; i < GAME_DIMENSION; i++) {
        bool row_win = row_consists_of_symbols(board, i, symbols);
        bool col_win = col_consists_of_symbols(board, i, symbols);
        if (row_win || col_win) {
            return true;
        }
    }

    return any_diagonal_consists_of_symbols(board, symbols);
}

bool is_victory_possible(char *board, char symbol)
{
    char symbols[3] = {symbol, ' ', '\0'};
    
    for (int i = 0; i < GAME_DIMENSION; i++) {
        bool row_playable = row_consists_of_symbols(board, i, symbols);
        bool col_playable = col_consists_of_symbols(board, i, symbols);
        if (row_playable || col_playable) {
            return true;
        }
    }

    return any_diagonal_consists_of_symbols(board, symbols);
}

static void print_row(char *buf, char *row, int *buf_offset)
{
    int idx = *buf_offset;
    int init_offset = *buf_offset;

    for (int col = 0; col < GAME_DIMENSION - 1; col++) {
        buf[idx++] = row[col];
        buf[idx++] = '|';
    }
    buf[idx++] = row[GAME_DIMENSION - 1];
    buf[idx++] = '\n';

    *buf_offset += idx - init_offset;
}

static void print_row_separator(char *buf, int *buf_offset)
{
    int idx = *buf_offset;
    int init_offset = *buf_offset;

    for (int i = 0; i < GAME_DIMENSION + GAME_DIMENSION - 1; i++) {
        buf[idx++] = '-';
    }
    buf[idx++] = '\n';

    *buf_offset += idx - init_offset;
}

char *printable_board(char *board)
{
    char *result = calloc_w(PRINTABLE_BOARD_LEN + 1, sizeof(char));
    int result_idx = 0;

    int row;
    for (row = 0; row < GAME_DIMENSION - 1; row++) {
        char *row_start = board + GAME_DIMENSION * row;
        print_row(result, row_start, &result_idx);
        print_row_separator(result, &result_idx);
    }
    print_row(result, board + GAME_DIMENSION * row, &result_idx);

    return result;
}
