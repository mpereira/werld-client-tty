#ifndef WERLD_PLAYER_H
#define WERLD_PLAYER_H

#define WERLD_PLAYER_NAME_BUFSIZ (20)

#include "movement.h"

struct player {
  int id;
  char name[WERLD_PLAYER_NAME_BUFSIZ];
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
