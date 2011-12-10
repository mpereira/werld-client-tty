#ifndef WERLD_MAP_H
#define WERLD_MAP_H

#include <stdint.h>
#include <stddef.h>

#include "tile.h"

struct map {
  uint32_t width;
  uint32_t height;
  struct tile **tiles;
};

size_t WERLD_MAP_TILES_SIZE(int, int);

void map_malloc(struct map **, int, int);
void map_free(struct map **);

#endif
