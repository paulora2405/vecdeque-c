#include "factory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part_text(Material *m, bool defective, size_t p_num, char **out_text) {
  *out_text = realloc(*out_text, sizeof(char) * 15);
  strcpy(*out_text, "ptt_0\0\0\0\0\0\0\0\0\0");
  char num[7];
  if(!defective) {
    strncpy(*out_text + 1, color_to_name(m->color), COLORNAME_STR_LEN);
    sprintf(num, "%ld", p_num);
    strcpy(*out_text + 4, num);
  } else {
    sprintf(num, "%ld", p_num);
    strcpy(*out_text, "d_");
    strcpy(*out_text + 2, num);
  }
}

void process_casts(Queue *list) {
  if(!list) {
    fprintf(stderr, "Error!\n");
    return;
  }

  Queue *correct_parts = NULL, *defective_parts = NULL;
  if(!queue_new(&correct_parts, sizeof(Material)) ||
     !queue_new(&defective_parts, sizeof(Material))) {
    fprintf(stderr, "Error!\n");
    return;
  }

  MaterialQueue *casts = NULL;
  queue_get(list, 0, (void **)&casts);

  Material *cast = NULL, *piece = NULL;
  MaterialQueue *mq = NULL;
  bool found = false;
  while(queue_pop_front(casts->queue, (void **)&cast)) {
    Color c = cast->color;
    found = false;
    for(size_t i = 1; i < queue_len(list); i++) {
      queue_get(list, i, (void **)&mq);
      if(mq->color == c) {
        found = true;
        queue_pop_front(mq->queue, (void **)&piece);
        queue_push_back(correct_parts, piece);
        break;
      }
    }
    if(!found) {
      queue_push_back(defective_parts, cast);
    }
  }

  Material *remain = NULL;
  while(queue_pop_front(list, (void **)&mq)) {
    while(queue_pop_front(mq->queue, (void **)&remain)) {
      queue_push_back(defective_parts, remain);
    }
  }

  // print everything
  char *part_str = NULL;
  Material *correct = NULL;
  printf("PC=[;");
  for(size_t i = 0; i < queue_len(correct_parts); i++) {
    queue_get(correct_parts, i, (void **)&correct);
    part_text(correct, false, i + 1, &part_str);
    printf("%s;", part_str);
  }
  printf("]\n");

  Material *defective = NULL;
  printf("PD=[;");
  for(size_t i = 0; i < queue_len(defective_parts); i++) {
    queue_get(defective_parts, i, (void **)&defective);
    part_text(defective, true, i + 1, &part_str);
    printf("%s;", part_str);
  }
  printf("]");

  queue_destroy(correct_parts);
  queue_destroy(defective_parts);
}

void materialqueue_print(MaterialQueue *mq) {
  if(!mq) {
    printf("Invalid MQ\n");
    return;
  }

  Queue *q = mq->queue;
  if(queue_is_empty(q)) {
    if(mq->color == NoColor)
      printf("Fm=[];\n");
    else
      printf("FC%s=[];\n", color_to_name(mq->color));
    return;
  }

  char *m_text = malloc(15 * sizeof(char));
  char type;
  Material *m = NULL;
  if(mq->color == NoColor)
    printf("Fm=["), type = 'M';
  else
    printf("FC%s=[", color_to_name(mq->color)), type = 'm';
  for(size_t i = 0; i < queue_len(q) - 1; i++) {
    queue_get(q, i, (void **)&m);
    material_to_text(m, m_text);
    printf("%c%s;", type, m_text);
  }
  queue_get(q, queue_len(q) - 1, (void **)&m);
  material_to_text(m, m_text);
  printf("m%s];\n", m_text);

  free(m_text);
  free(m);
}

bool materialqueue_from_str(char str[], MaterialQueue **out_mq) {
  char *t = NULL, *s = NULL;
  t = strtok_r(str, "=", &s);
  if(t == NULL)
    return materialqueue_new(NoColor, out_mq);

  char mq_c[COLORNAME_STR_LEN + 1];
  size_t len = strlen(t);
  if(len == 2) {
    materialqueue_new(NoColor, out_mq);
  } else if(len == 4) {
    strncpy(mq_c, &t[COLORNAME_STR_LEN], COLORNAME_STR_LEN);
    mq_c[COLORNAME_STR_LEN] = '\0';
    materialqueue_new(color_from_name(mq_c), out_mq);
  }

  t = strtok_r(NULL, "[]", &s);
  if(strlen(t) == 0)
    return false;

  t = strtok_r(t, ";", &s);
  char *m_c = malloc(sizeof(char) * 3);
  short m_n;
  char trash = ' ';
  while(t != NULL) {
    if(sscanf(t, "%c%2s%hd", &trash, m_c, &m_n) != 3) {
      free(m_c);
      return false;
    }

    Material m = material_new(color_from_name(m_c), m_n);
    queue_push_back((*out_mq)->queue, &m);

    t = strtok_r(NULL, ";", &s);
  }

  free(m_c);

  return true;
}

bool materialqueue_new(Color color, MaterialQueue **out_mq) {
  *out_mq = realloc(*out_mq, sizeof(MaterialQueue));
  (*out_mq)->color = color;
  if(!queue_new(&(*out_mq)->queue, sizeof(Material)))
    return false;
  return true;
}

void materialqueue_destroy(MaterialQueue *mq) {
  if(!mq)
    return;
  queue_destroy(mq->queue);
  free(mq);
}

bool materialqueue_list_parse(Queue **out_list) {
  char *lines[MAX_NUM_LINES];
  int n = read_stdin(lines) - 1;  // - N=3;\n Pt=[];
  if(n < 2)
    return false;

  if(!queue_new(out_list, sizeof(MaterialQueue)))
    return false;

  MaterialQueue *mq = NULL;
  for(int i = 0; i < n; i++) {
    materialqueue_from_str(lines[i + 1], &mq);
    Queue *q = *out_list;
    if(!queue_push_back(q, mq))
      fprintf(stderr, "Error pushing\n");
  }

  return true;
}

int read_stdin(char **lines) {
  char line[MAX_LINE_LEN];
  int num_lines = 0;

  while(fgets(line, MAX_LINE_LEN, stdin) != NULL && num_lines < MAX_NUM_LINES) {
    line[strcspn(line, "\n")] = '\0';

    lines[num_lines] = (char *)malloc(strlen(line) + 1);
    if(lines[num_lines] == NULL) {
      fprintf(stderr, "malloc failed");
      exit(EXIT_FAILURE);
    }

    strcpy(lines[num_lines], line);
    num_lines++;
  }

  return num_lines;
}
