#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "tile.h"

size_t WERLD_MAP_TILES_SIZE(int width, int height) {
  return(sizeof(struct tile) * width * height);
}

void map_malloc(struct map **map, int width, int height) {
  if (!(*map = malloc(sizeof(**map)))) {
    perror("malloc");
    exit(errno);
  }
  if (!((*map)->tiles = malloc(width * sizeof((*(*map)->tiles))))) {
    perror("malloc");
    exit(errno);
  }
  for (int i = 0; i < width; i++) {
    if (!((*map)->tiles[i] = malloc(height * sizeof((**(*map)->tiles))))) {
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
