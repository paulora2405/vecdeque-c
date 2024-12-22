#ifndef COLOR_H
#define COLOR_H

#define COLORNAME_STR_LEN 2

typedef enum Color {
  Blue,
  Yellow,
  Indigo,
  Black,
  White,
  Green,
  Red,
  Lilac,
  Pink,
  Orange,
  NoColor,
} Color;

char *color_to_name(Color color);
Color color_from_name(char *name);

#endif  // COLOR_H
