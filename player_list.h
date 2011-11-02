#ifndef WERLD_PLAYER_LIST_H
#define WERLD_PLAYER_LIST_H

#include "player.h"

struct player_list {
  struct player *player;
  struct player_list *next;
};

extern struct player_list *player_list;

void player_list_init(struct player_list **);
void player_list_free(struct player_list *);
void player_list_update(struct player_list **, const struct player *, int);
void player_list_insert(struct player_list **, struct player);

#endif
