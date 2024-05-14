#ifndef SESSION
#define SESSION 1

#define SESSION_ST_IDLE 0
#define SESSION_ST_HOST 1
#define SESSION_ST_GUEST 2

#include "../clients/connection.h"

typedef struct session {
    connection_t *connection;
    char *id;
    char *nickname;
    char *game_id;
    char symbol;
    int status;
} session_t;

void free_session(session_t *session);
session_t *create_session();

#endif
