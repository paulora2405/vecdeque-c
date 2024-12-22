#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "queue.h"

typedef struct Material {
  Color color;
  short number;
} Material;

Material material_new(Color color, short number);
void material_to_text(Material *material, char *out_text);

#endif  // MATERIAL_H
