#ifndef WERLD_TILE_H
#define WERLD_TILE_H

#include <stdint.h>

extern uint8_t WERLD_TILE_TYPE_DIRT;
extern uint8_t WERLD_TILE_TYPE_GRASS;
extern uint8_t WERLD_TILE_TYPE_SAND;
extern uint8_t WERLD_TILE_TYPE_SNOW;
extern uint8_t WERLD_TILE_TYPE_WATER;
extern uint8_t WERLD_TILE_TYPE_STONE;
extern uint8_t WERLD_TILE_TYPE_WALL;

extern unsigned short int WERLD_TILE_PROPERTY_TRAVERSABLE;

struct tile {
  uint8_t type;
  uint8_t properties;
};

#endif
