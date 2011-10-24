#ifndef WERLD_CLIENT_PLAYER_H
#define WERLD_CLIENT_PLAYER_H

#define MAX_NAME_SIZE 20

#include <curses.h>

#include "movement.h"

struct player {
  int id;
  char name[MAX_NAME_SIZE];
  int y;
  int x;
};

void player_initialize(struct player *,
                       const int,
                       const char *,
                       const int,
                       const int);
void player_free(struct player *);
void player_move(struct player *, enum movement);

struct player player;

#endif
