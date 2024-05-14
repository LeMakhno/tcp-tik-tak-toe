#ifndef _LINKED_LIST
#define _LINKED_LIST 1

#include "../../include/common.h"

typedef bool (*llist_value_equalizer_t)(void *, void *);
typedef struct linked_list_node llist_node_t;

typedef struct linked_list {
    llist_node_t *first;
    llist_node_t *last;
    llist_node_t *current;
    llist_value_equalizer_t value_equalizer;
    int size;
} llist_t;

struct linked_list_node {
    void *value;
    llist_node_t *next;
    llist_node_t *prev;
    llist_t *llist;
};

void llist_init(llist_t **list_ptr, llist_value_equalizer_t value_equalizer);
void llist_add(llist_t *llist, void *value);
llist_node_t *llist_get(llist_t *llist, int idx);
llist_node_t *llist_contains(llist_t *llist, void *value);
llist_node_t *llist_remove(llist_t *llist, void *value);
void llist_detach(llist_t *llist, llist_node_t *node);
void llist_free(llist_t *llist, bool dealloc_values);

void llist_iterate(llist_t *llist);
bool llist_hasnext(llist_t *llist);
llist_node_t *llist_next(llist_t *llist);

#endif
