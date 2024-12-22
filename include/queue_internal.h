#ifndef QUEUE_INTERNAL_H
#define QUEUE_INTERNAL_H

#include "queue.h"

#define QUEUE_MIN_CAP 2

// Internal Queue structure
typedef struct _Queue {
  size_t cap;     // Queue Capacity
  size_t i_size;  // Item Size
  size_t len;     // Quantity of Items
  size_t head;    // Index of Item on Start of Queue (First to be popped)
  void *buf;      // Buffer to raw data
} _Queue;

bool queue_get_physical(Queue *queue, size_t index, void **out_item);

bool queue_grow(Queue *queue);

bool queue_is_full(Queue *queue);

bool queue_reserve_for_push(Queue *queue);

void queue_handle_cap_increase(Queue *queue, size_t old_capacity);

size_t queue_to_physical_idx(Queue *queue, size_t index);

size_t queue_tail(Queue *queue);

size_t queue_wrap_add(size_t index, size_t addend, size_t capacity);

#endif  // QUEUE_INTERNAL_H
