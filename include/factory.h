#ifndef FACTORY_H
#define FACTORY_H

#include "material.h"

typedef struct MaterialQueue {
  Color color;
  Queue *queue;
} MaterialQueue;

void process_casts(Queue *materialqueue_list);

void materialqueue_print(MaterialQueue *materialqueue);
bool materialqueue_from_str(char str[], MaterialQueue **out_materialqueue);
bool materialqueue_new(Color color, MaterialQueue **out_materialqueue);
void materialqueue_destroy(MaterialQueue *materialqueue);
bool materialqueue_list_parse(Queue **out_materialqueue_list);

#define MAX_LINE_LEN 4096
#define MAX_NUM_LINES 50

int read_stdin(char **lines);

#endif  // FACTORY_H
