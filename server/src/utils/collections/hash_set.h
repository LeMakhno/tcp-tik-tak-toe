#ifndef _HASH_SET
#define _HASH_SET 1

#include "hash_map.h"

typedef struct hset {
    hmap_t *__map;
    int size;
} hset_t;

void hset_init(
    hset_t **hset,
    int table_size,
    keysize_calculator_t keysize_calculator,
    key_comparator_t key_comparator
);
void hset_add(hset_t *hset, void *key);
void hset_remove(hset_t *hset, void *key);
bool hset_contains(hset_t *hset, void *key);
void hset_free(hset_t *hset, bool dealloc_content);

#endif
