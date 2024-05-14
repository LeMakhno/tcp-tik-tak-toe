#include "session.h"
#include "../utils/generic_utils.h"
#include "../utils/string_utils.h"

void free_session(session_t *session)
{
    if (!session) {
        return;
    }
    if (session->nickname) {
        free(session->nickname);
    }
    free(session->id);
    free(session);
}

session_t *create_session()
{
    session_t *session = calloc_w(1, sizeof(session_t));
    session->id = random_id(20);
    session->status = SESSION_ST_IDLE;

    return session;
}
