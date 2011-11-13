#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#include "message_list.h"
#include "player.h"
#include "player_message_list.h"

void player_message_list_malloc(struct player_message_list **player_message_list) {
  if (!(*player_message_list = malloc(sizeof(struct player_message_list)))) {
    perror("malloc");
    exit(errno);
  }
}

void player_message_list_init(struct player_message_list **player_message_list) {
  *player_message_list = NULL;
}

int player_message_list_add(struct player_message_list **player_message_list,
                            const char *message,
                            int player_id) {
  time_t now;
  time(&now);

  if (!*player_message_list) {
    player_message_list_malloc(player_message_list);
    (*player_message_list)->player_id = player_id;
    (*player_message_list)->message_list = NULL;
    (*player_message_list)->next = NULL;
    return(message_list_add(&((*player_message_list)->message_list), message, now));
  }

  struct player_message_list *iterator = *player_message_list;
  struct player_message_list *previous;

  for (; iterator; iterator = iterator->next) {
    previous = iterator;
    if (iterator->player_id == player_id) {
      return(message_list_add(&(iterator->message_list), message, now));
    }
  }

  player_message_list_malloc(&(previous->next));
  previous->next->player_id = player_id;
  previous->next->message_list = NULL;
  previous->next->next = NULL;
  return(message_list_add(&(previous->next->message_list), message, now));
}

int player_message_list_remove(struct player_message_list **player_message_list,
                               const char *message,
                               int player_id) {
  struct player_message_list *iterator = *player_message_list;

  for (; iterator; iterator = iterator->next) {
    if (iterator->player_id == player_id) {
      return(message_list_remove(&(iterator->message_list), message));
    }
  }

  return(0);
}

const struct player_message_list *
player_message_list_find_by_player(struct player_message_list *player_message_list,
                                   struct player player) {
  for (; player_message_list; player_message_list = player_message_list->next) {
    if (player_message_list->player_id == player.id) {
      return(player_message_list);
    }
  }

  return(NULL);
}

void player_message_list_free(struct player_message_list *player_message_list) {
  struct player_message_list *tmp;

  while (player_message_list) {
    tmp = player_message_list;
    message_list_free(player_message_list->message_list);
    player_message_list = player_message_list->next;
    free(tmp);
  }
}
