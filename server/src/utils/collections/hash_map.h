#ifndef _HASH_MAP
#define _HASH_MAP 1

#include "linked_list.h"
#include "../../include/common.h"

typedef int (*keysize_calculator_t)(void *);
typedef bool (*key_comparator_t)(void *, void *);

int string_keysize(void *str);
int int_keysize(void *num);
bool string_key(void *a, void *b);
bool int_key(void *a, void *b);

typedef struct hmap {
    llist_t **table;
    int table_size;
    keysize_calculator_t keysize_calculator;
    int size;
} hmap_t;

typedef struct hmap_entry {
    void *key;
    void *value;
} hmap_entry_t;

void hmap_init(
    hmap_t **hmap,
    int table_size,
    keysize_calculator_t keysize_calculator,
    key_comparator_t key_comparator
);
void *hmap_put(hmap_t *hmap, void *key, void *value);
void *hmap_remove(hmap_t *hmap, void *key);
void *hmap_get(hmap_t *hmap, void *key);
void hmap_free(hmap_t *hmap, bool dealloc_keys, bool dealloc_values);

#endif
