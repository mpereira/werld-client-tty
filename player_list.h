#ifndef WERLD_PLAYER_LIST_H
#define WERLD_PLAYER_LIST_H

#include <stdint.h>

#include "player.h"

struct player_list {
  struct player *player;
  struct message_list *message_list;
  struct player_list *next;
};

void player_list_malloc(struct player_list **);
void player_list_init(struct player_list **);
void player_list_free(struct player_list *);
void player_list_add_player(struct player_list **, const struct player *);
void player_list_update_player(struct player_list **, struct player *);
void player_list_update(struct player_list **, const struct player *, int);
int player_list_add_message(struct player_list **, const char *, uint32_t);
int player_list_remove_message(struct player_list **, const char *, uint32_t);

#endif
