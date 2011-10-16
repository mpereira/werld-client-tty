#ifndef WERLD_CLIENT_PLAYER_H
#define WERLD_CLIENT_PLAYER_H

#define MAX_NAME_SIZE 20
#define MAX_SAY_SIZE 50

#include <curses.h>

#include "movement.h"

struct player {
  int id;
  char *name;
  int y;
  int x;
};

void player_initialize(struct player *, int, char *, int, int);
void player_move(struct player *, enum movement);

struct player player;

#endif
