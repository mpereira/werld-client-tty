#ifndef WERLD_TILE_H
#define WERLD_TILE_H

#include <stdint.h>

extern uint8_t WERLD_TILE_TYPE_GROUND;
extern uint8_t WERLD_TILE_TYPE_WALL;

struct tile {
  uint8_t type;
  uint8_t properties;
};

#endif
