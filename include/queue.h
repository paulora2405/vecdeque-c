#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

// Obscure Queue Structure
typedef struct _Queue Queue;

void debug_print_char(Queue *q);

void debug_print_physical_queue(Queue *q);

bool queue_push_back(Queue *queue, void *item);

bool queue_push_front(Queue *queue, void *item);

bool queue_pop_front(Queue *q, void **out_item);

bool queue_pop_back(Queue *q, void **out_item);

bool queue_get(Queue *queue, size_t index, void **out_item);

bool queue_is_empty(Queue *q);

size_t queue_len(Queue *q);

bool queue_new(Queue **out_queue, size_t item_size);

bool queue_new_with_capacity(Queue **out_queue, size_t item_size, size_t capacity);

bool queue_destroy(Queue *queue);

#endif  // QUEUE_H
