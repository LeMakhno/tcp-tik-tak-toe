#include "storage.h"

#include "../utils/collections/hash_map.h"
#include "../utils/collections/hash_set.h"

static const int BUCKETS = 64;

static hmap_t *sessions_storage = NULL;
static hmap_t *game_storage = NULL;
static hset_t *nicknames_set = NULL;

void storage_init()
{
    hmap_init(&sessions_storage, BUCKETS, string_keysize, string_key);
    hmap_init(&game_storage, BUCKETS, string_keysize, string_key);
    hset_init(&nicknames_set, BUCKETS, string_keysize, string_key);
}

/* Games */
void add_game(game_t *game)
{
    if (!game || !game->id) {
        return;
    }
    hmap_put(game_storage, game->id, game);
}

game_t *find_game(char *id)
{
    if (!id) {
        return NULL;
    }
    return hmap_get(game_storage, id);
}

game_t *remove_game(char *id)
{
    if (!id) {
        return NULL;
    }
    return hmap_remove(game_storage, id);
}

/* Nicknames */
void add_nickname(char *nickname)
{
    if (!nickname) {
        return;
    }
    hset_add(nicknames_set, nickname);
}

bool is_nickname_taken(char *nickname)
{
    if (!nickname) {
        return true;
    }
    return hset_contains(nicknames_set, nickname);
}

void remove_nickname(char *nickname)
{
    if (!nickname) {
        return;
    }
    hset_remove(nicknames_set, nickname);
}

/* Sessions */
void add_session(session_t *session)
{
    if (!session || !session->id) {
        return;
    }
    hmap_put(sessions_storage, session->id, session);
}

session_t *find_session(char *id)
{
    if (!id) {
        return NULL;
    }
    return hmap_get(sessions_storage, id);
}

session_t *remove_session(char *id)
{
    if (!id) {
        return NULL;
    }
    return hmap_remove(sessions_storage, id);
}
