#ifndef WERLD_MAP_H
#define WERLD_MAP_H

#include <stdint.h>
#include <stddef.h>

struct map {
  uint32_t width;
  uint32_t height;
  uint8_t **tiles;
};

extern uint8_t WERLD_MAP_TILE_TYPE_GROUND;
extern uint8_t WERLD_MAP_TILE_TYPE_WALL;

size_t WERLD_MAP_TILES_SIZE(int, int);

void map_malloc(struct map **, int, int);
void map_free(struct map **);

#endif
