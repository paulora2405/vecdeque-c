#include "queue_internal.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void debug_print_char(Queue *q) {
  if(!q)
    return;

  size_t len = queue_len(q);
  printf("[ ");
  char *x = malloc(sizeof(char));
  for(size_t i = 0; i < len; i++) {
    queue_get(q, i, (void **)&x);
    printf("%c ", *x);
  }
  printf("]\n");
  free(x);
}

void debug_print_physical_queue(Queue *q) {
  if(!q)
    return;

  printf("cap=%ld\tlen=%ld\thead=%ld\ttail=%ld\tbuf=[ ", q->cap, q->len, q->head, queue_tail(q));
  char *x = malloc(sizeof(char));
  *x = '\0';
  for(size_t i = 0; i < q->cap; i++) {
    queue_get_physical(q, i, (void **)&x);
    if(*x == '\0' || *x > 127 || *x < 0)
      *x = '.';
    printf("%c ", *x);
  }
  printf("]\n");
  free(x);
}

bool queue_push_back(Queue *q, void *item) {
  if(!q || !item)
    return false;
  if(queue_is_full(q))
    if(!queue_grow(q))
      return false;
  size_t offset = queue_tail(q) * q->i_size;
  q->len++;
  memcpy(((uint8_t *)q->buf) + offset, item, q->i_size);
  return true;
}

bool queue_push_front(Queue *q, void *item) {
  if(!q || !item)
    return false;
  if(queue_is_full(q))
    if(!queue_grow(q))
      return false;
  q->head = queue_to_physical_idx(q, q->cap - 1);
  size_t offset = q->head * q->i_size;
  q->len++;
  memcpy(((uint8_t *)q->buf) + offset, item, q->i_size);
  return true;
}

bool queue_pop_front(Queue *q, void **out_item) {
  if(queue_is_empty(q))
    return false;

  *out_item = realloc(*out_item, q->i_size);
  size_t old_head = q->head;
  q->head = queue_to_physical_idx(q, 1);
  if(--q->len == 0)
    q->head = 0;
  size_t offset = old_head * q->i_size;
  memcpy(*out_item, ((uint8_t *)q->buf) + offset, q->i_size);
  memset(((uint8_t *)q->buf) + offset, 0, q->i_size);
  return true;
}

bool queue_pop_back(Queue *q, void **out_item) {
  if(queue_is_empty(q))
    return false;

  *out_item = realloc(*out_item, q->i_size);
  size_t old_tail = queue_tail(q);
  if(--q->len == 0)
    q->head = 0;
  size_t offset = old_tail * q->i_size;
  memcpy(*out_item, ((uint8_t *)q->buf) + offset, q->i_size);
  memset(((uint8_t *)q->buf) + offset, 0, q->i_size);
  return true;
}

bool queue_get(Queue *q, size_t idx, void **out_item) {
  if(!q || idx >= q->len)
    return false;
  size_t physical_idx = queue_to_physical_idx(q, idx);
  return queue_get_physical(q, physical_idx, out_item);
}

bool queue_get_physical(Queue *q, size_t idx, void **out_item) {
  if(queue_is_empty(q) || idx >= q->cap)
    return false;

  *out_item = realloc(*out_item, q->i_size);
  size_t offset = idx * q->i_size;
  memcpy(*out_item, ((uint8_t *)q->buf) + offset, q->i_size);
  return true;
}

bool queue_grow(Queue *q) {
  size_t old_cap = q->cap;
  if(!queue_reserve_for_push(q))
    return false;
  queue_handle_cap_increase(q, old_cap);
  return true;
}

bool queue_is_full(Queue *q) {
  return q->len == q->cap;
}

bool queue_reserve_for_push(Queue *q) {
  size_t required_cap = q->cap + 1;
  size_t double_cap = q->cap * 2;
  size_t new_cap = (double_cap > required_cap) ? double_cap : required_cap;
  q->buf = realloc(q->buf, new_cap * q->i_size);
  if(!q->buf)
    return false;
  q->cap = new_cap;
  return true;
}

void queue_handle_cap_increase(Queue *q, size_t old_cap) {
  // Move the shortest contiguous section of the ring buffer
  //
  // H := head
  // L := last element (`queue_to_physical_idx(q.len - 1)`)
  //
  //     H             L
  //   [ o o o o o o o o ]
  //     H             L
  // A [ o o o o o o o o . . . . . . . . ]
  //
  //         L H
  //   [ o o o o o o o o ]
  //           H             L
  // B [ . . . o o o o o o o o . . . . . ]
  //
  //               L H
  //   [ o o o o o o o o ]
  //               L                 H
  // C [ o o o o o o . . . . . . . . o o ]

  size_t new_cap = q->cap;
  if(q->head <= old_cap - q->len) {
    // A
    // NOP
  } else {
    size_t head_len = old_cap - q->head;
    size_t tail_len = q->len - head_len;
    if(head_len > tail_len && new_cap - old_cap >= tail_len) {
      // B
      size_t offset = old_cap * q->i_size;
      size_t bytes = tail_len * q->i_size;
      // Memcpy is fine here because segments are guaranteed non overlapping
      memcpy(((uint8_t *)q->buf) + offset, q->buf, bytes);
      memset(q->buf, 0, bytes);
    } else {
      // C
      size_t new_head = new_cap - head_len;
      size_t offset_dst = new_head * q->i_size;
      size_t offset_src = q->head * q->i_size;
      size_t bytes = head_len * q->i_size;
      // Memmove here because memory can be overlapping
      //               L H
      //   [ o o o o o o o o ]
      //               L   H
      // C [ o o o o o o . o o ]
      memmove(((uint8_t *)q->buf) + offset_dst, ((uint8_t *)q->buf) + offset_src, bytes);
      memset(((uint8_t *)q->buf) + offset_src, 0, bytes);
      q->head = new_head;
    }
  }
}

size_t queue_to_physical_idx(Queue *q, size_t idx) {
  // Assumes `q` is valid
  return queue_wrap_add(q->head, idx, q->cap);
}

size_t queue_tail(Queue *q) {
  // Assumes `q` is valid
  return queue_to_physical_idx(q, q->len);
}

size_t queue_wrap_add(size_t idx, size_t addend, size_t cap) {
  size_t logical_idx = idx + addend;
  return logical_idx >= cap ? logical_idx - cap : logical_idx;
}

bool queue_is_empty(Queue *q) {
  return queue_len(q) == 0;
}

size_t queue_len(Queue *q) {
  if(!q)
    return 0;
  return q->len;
}

bool queue_new(Queue **out_q, size_t size) {
  return queue_new_with_capacity(out_q, size, QUEUE_MIN_CAP);
}

bool queue_new_with_capacity(Queue **out_q, size_t size, size_t cap) {
  cap = (cap > QUEUE_MIN_CAP) ? cap : QUEUE_MIN_CAP;
  *out_q = (Queue *)malloc(sizeof(Queue));
  if(!*out_q)
    return false;
  (*out_q)->head = 0;
  (*out_q)->len = 0;
  (*out_q)->i_size = size;
  (*out_q)->cap = cap;
  (*out_q)->buf = malloc(cap * size);
  if(!(*out_q)->buf)
    return false;
  memset((*out_q)->buf, 0, cap * size);
  return true;
}

bool queue_destroy(Queue *q) {
  if(!q)
    return false;
  if(q->buf) {
    memset(q->buf, 0, q->cap * q->i_size);
    free(q->buf);
  }
  memset(q, 0, sizeof(Queue));
  free(q);
  return true;
}
