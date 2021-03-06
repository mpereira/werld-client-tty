#ifndef WERLD_PLAYER_H
#define WERLD_PLAYER_H

#include <stdint.h>

#include "movement.h"

enum { WERLD_PLAYER_NAME_MAX_SIZE = 20 };

struct player {
  uint32_t id;
  char name[WERLD_PLAYER_NAME_MAX_SIZE];
  uint32_t y;
  uint32_t x;
};

void player_malloc(struct player **);
void player_free(struct player **);
void player_set(struct player *, int, const char *, uint32_t, uint32_t);
int player_move(struct player *, enum movement);

#endif
