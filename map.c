#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "map.h"

uint8_t WERLD_MAP_TILE_TYPE_GROUND = 0;
uint8_t WERLD_MAP_TILE_TYPE_WALL   = 1;

size_t WERLD_MAP_TILES_SIZE(int width, int height) {
  return(sizeof(uint8_t) * width * height);
}

void map_malloc(struct map **map, int width, int height) {
  if (!(*map = malloc(sizeof(struct map)))) {
    perror("malloc");
    exit(errno);
  }
  if (!((*map)->tiles = malloc(width * sizeof(uint8_t *)))) {
    perror("malloc");
    exit(errno);
  }
  for (int i = 0; i < width; i++) {
    if (!((*map)->tiles[i] = malloc(height * sizeof(uint8_t)))) {
      perror("malloc");
      exit(errno);
    }
  }
}

void map_free(struct map **map) {
  for (int i = 0; (uint32_t) i < (*map)->width; i++) free((*map)->tiles[i]);
  free((*map)->tiles);
  free(*map);
  *map = NULL;
}
