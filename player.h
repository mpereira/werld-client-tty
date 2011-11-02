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

extern struct player player;

void player_malloc(struct player **);
void player_initialize(struct player *, int, const char *, int, int);
void player_cpy(struct player **, struct player *);
void player_free(struct player *);
void player_move(struct player *, enum movement);

#endif
