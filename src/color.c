#include "color.h"

#include <stdlib.h>
#include <string.h>

char *color_to_name(Color color) {
  switch(color) {
    case Blue:
      return "az";  // Azul
    case Yellow:
      return "am";  // Amarelo
    case Indigo:
      return "an";  // Anil
    case Black:
      return "pr";  // Preto
    case White:
      return "br";  // Branco
    case Green:
      return "ve";  // Verde
    case Red:
      return "vo";  // Vermelho
    case Lilac:
      return "li";  // Lilás
    case Pink:
      return "ro";  // Rosa
    case Orange:
      return "lj";  // Laranja
    case NoColor:
    default:
      return "nc";
  }
}

Color color_from_name(char *name) {
  if(name == NULL)
    return 0;

  if(strcmp("az", name) == 0)
    return Blue;
  else if(strcmp("am", name) == 0)
    return Yellow;
  else if(strcmp("an", name) == 0)
    return Indigo;
  else if(strcmp("pr", name) == 0)
    return Black;
  else if(strcmp("br", name) == 0)
    return White;
  else if(strcmp("ve", name) == 0)
    return Green;
  else if(strcmp("vo", name) == 0)
    return Red;
  else if(strcmp("li", name) == 0)
    return Lilac;
  else if(strcmp("ro", name) == 0)
    return Pink;
  else if(strcmp("lj", name) == 0)
    return Orange;

  return 0;
}
