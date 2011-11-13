#ifndef WERLD_PLAYER_MESSAGE_LIST_H
#define WERLD_PLAYER_MESSAGE_LIST_H

#include "message_list.h"
#include "player.h"

struct player_message_list {
  int player_id;
  struct message_list *message_list;
  struct player_message_list *next;
};

void player_message_list_malloc(struct player_message_list **);
void player_message_list_init(struct player_message_list **);
int player_message_list_add(struct player_message_list **, const char *, int);
int player_message_list_remove(struct player_message_list **, const char *, int);

const struct player_message_list *
player_message_list_find_by_player(struct player_message_list *, struct player);

void player_message_list_free(struct player_message_list *);

#endif
