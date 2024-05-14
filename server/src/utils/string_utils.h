#ifndef _STRING_UTILS
#define _STRING_UTILS 1

#include "../include/common.h"

typedef struct string_arr {
    int elements_num;
    char **strings;
} string_arr_t;

char *random_id(int len);
char random_symbol(char *chars);
int count_chars(char *str, char chr);
/**
 * Returns new allocated strings
*/
string_arr_t split_string(char *str, char chr);
void free_string_arr(string_arr_t split);
int trim_end(char *str, char *chars);
char *trim(char *str);
bool streq(char *a, char *b);

#endif
