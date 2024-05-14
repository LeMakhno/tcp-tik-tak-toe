#ifndef _LINKED_QUEUE
#define _LINKED_QUEUE 1

#include "../../include/common.h"

typedef struct queue_entry
{
    void *value;
    struct queue_entry *next;
    struct queue_entry *prev;
}
queue_entry_t;

typedef struct linked_queue
{
    queue_entry_t *first;
    queue_entry_t *last;
    size_t size;
}
linked_queue_t;

void queue_init(linked_queue_t **queue);
void free_queue(linked_queue_t *queue, bool dealloc_values);

void queue_put(linked_queue_t *queue, void *new_value);
void *queue_poll(linked_queue_t *queue);

#endif
