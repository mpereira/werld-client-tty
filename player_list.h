#ifndef WERLD_CLIENT_PLAYER_LIST_H
#define WERLD_CLIENT_PLAYER_LIST_H

#include "player.h"

struct player_list {
  struct player *player;
  struct player_list *next;
};

void player_list_init(struct player_list **);
void player_list_free(struct player_list *);
void player_list_fill(struct player_list **, const struct player *, const int);

#endif
