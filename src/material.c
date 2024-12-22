#include "material.h"

#include <stdio.h>
#include <string.h>

Material material_new(Color c, short n) {
  Material m = {c, n};
  return m;
}

void material_to_text(Material *m, char *out_t) {
  if(!m)
    return;

  char num[7];
  sprintf(num, "%d", m->number);
  strcpy(out_t, color_to_name(m->color));
  strcat(out_t, num);
}
