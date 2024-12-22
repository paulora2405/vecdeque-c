#include <stdio.h>
#include <stdlib.h>

#include "factory.h"

// Test Queue handle capacity increase scenario B
void test_scenario_B(void) {
  Queue *q = NULL;
  printf("Creating Queue\n");
  if(!queue_new_with_capacity(&q, sizeof(char), 5))
    fprintf(stderr, "Create Error!\n");
  debug_print_physical_queue(q);

  printf("Pushing to back of Queue\n");
  char item = '0';
  for(int i = 0; i < 5; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Popping from front of Queue\n");
  char *out = NULL;  // (char *)malloc(sizeof(char));
  for(int i = 0; i < 2; i++) {
    if(!queue_pop_front(q, (void **)&out))
      fprintf(stderr, "Push Error %d!\n", i);
    debug_print_physical_queue(q);
  }

  printf("Pushing to back of Queue\n");
  for(int i = 0; i < 4; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }
  printf("Destroying Queue\n");
  free(out);
  queue_destroy(q);
}

// Test Queue handle capacity increase scenario C
void test_scenario_C(void) {
  Queue *q = NULL;
  printf("Creating Queue\n");
  if(!queue_new_with_capacity(&q, sizeof(char), 5))
    fprintf(stderr, "Create Error!\n");
  debug_print_physical_queue(q);

  printf("Pushing to back of Queue\n");
  char item = '0';
  for(int i = 0; i < 5; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Popping from front of Queue\n");
  char *out = (char *)malloc(sizeof(char));
  for(int i = 0; i < 3; i++) {
    if(!queue_pop_front(q, (void **)&out))
      fprintf(stderr, "Push Error %d!\n", i);
    debug_print_physical_queue(q);
  }

  printf("Pushing to back of Queue\n");
  for(int i = 0; i < 4; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Destroying Queue\n");
  free(out);
  queue_destroy(q);
}

// Test Queue handle capacity increase in general
void test_queue(void) {
  Queue *q = NULL;
  printf("Creating Queue\n");
  if(!queue_new_with_capacity(&q, sizeof(char), 2))
    fprintf(stderr, "Create Error!\n");
  debug_print_physical_queue(q);

  printf("Pushing to back of Queue\n");
  char item = '0';
  for(int i = 0; i < 8; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Popping from front of Queue\n");
  char *out = (char *)malloc(sizeof(char));
  for(int i = 0; i < 6; i++) {
    if(!queue_pop_front(q, (void **)&out))
      fprintf(stderr, "Pop Error %d!\n", i);
    debug_print_physical_queue(q);
  }

  printf("Pushing to back of Queue\n");
  for(int i = 0; i < 8; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Popping from front of Queue\n");
  while(queue_pop_front(q, (void **)&out)) {
    debug_print_physical_queue(q);
  }

  printf("Pushing to back of Queue\n");
  for(int i = 0; i < 4; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  free(out);
  printf("Destroying Queue\n");
  queue_destroy(q);
}

void test_push_front(void) {
  Queue *q = NULL;
  printf("Creating Queue\n");
  if(!queue_new_with_capacity(&q, sizeof(char), 2))
    fprintf(stderr, "Create Error!\n");
  debug_print_physical_queue(q);

  printf("Pushing to back of Queue\n");
  char item = '0';
  for(int i = 0; i < 6; i++) {
    if(!queue_push_back(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Pushing to front of Queue\n");
  for(int i = 0; i < 6; i++) {
    if(!queue_push_front(q, &item))
      fprintf(stderr, "Push Error %d!\n", i);
    item++;
    debug_print_physical_queue(q);
  }

  printf("Popping from front of Queue\n");
  char *out = (char *)malloc(sizeof(char));
  for(int i = 0; i < 4; i++) {
    if(!queue_pop_front(q, (void **)&out))
      fprintf(stderr, "Pop Error %d!\n", i);
    debug_print_physical_queue(q);
  }

  printf("Popping from back of Queue\n");
  for(int i = 0; i < 4; i++) {
    if(!queue_pop_back(q, (void **)&out))
      fprintf(stderr, "Pop Error %d!\n", i);
    debug_print_physical_queue(q);
  }

  free(out);
  printf("Destroying Queue\n");
  queue_destroy(q);
}

// Test MaterialQueue creation, insertion and printing
void test_materialqueue(void) {
  MaterialQueue *mq = NULL;
  if(!materialqueue_new(Blue, &mq))
    fprintf(stderr, "MQ creation error\n");
  Material m = material_new(Black, 5);
  queue_push_back(mq->queue, &m);
  m = material_new(Blue, 8);
  queue_push_back(mq->queue, &m);
  materialqueue_print(mq);
  materialqueue_destroy(mq);
}

// Test MaterialQueue parsing from string
void test_materialqueue_parse(void) {
  char in[] = "FCve=[;mve1;mve2;mve3;];";
  MaterialQueue *mq = NULL;
  materialqueue_from_str(in, &mq);
  materialqueue_print(mq);
  materialqueue_destroy(mq);
}

// Test Stdin parse of multiple MaterialQueues
void test_stdin_parse(void) {
  Queue *list = NULL;
  materialqueue_list_parse(&list);
  MaterialQueue *mq = NULL;
  for(size_t i = 0; i < queue_len(list); i++) {
    if(!queue_get(list, i, (void **)&mq)) {
      fprintf(stderr, "Error getting\n");
    }
    materialqueue_print(mq);
  }
}

// Solve stdin for factory problem
void solve(void) {
  Queue *list = NULL;
  materialqueue_list_parse(&list);
  process_casts(list);
  MaterialQueue *mq = NULL;
  for(size_t i = 0; i < queue_len(list); i++) {
    queue_get(list, i, (void **)&mq);
    materialqueue_destroy(mq);
  }
  queue_destroy(list);
}

int main(void) {
  test_scenario_B();
  test_scenario_C();
  test_queue();
  test_push_front();
  // test_materialqueue();
  // test_materialqueue_parse();
  // test_stdin_parse();
  // solve();
  return 0;
}
