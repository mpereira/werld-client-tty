#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "player_list.h"

struct player_list *player_list;

static void player_list_malloc(struct player_list **player_list) {
  if (!(*player_list = malloc(sizeof(struct player_list)))) {
    perror("malloc");
    exit(errno);
  }
}

void player_list_init(struct player_list **player_list) {
  *player_list = NULL;
}

void player_list_free(struct player_list *player_list) {
  struct player_list *current = player_list;
  struct player_list *previous;

  while(current) {
    if (current->player) {
      player_free(current->player);
    }
    previous = current;
    current = current->next;
    free(previous);
  }
}

void player_list_update(struct player_list **player_list, const struct player *players, int number_of_players) {
  if (number_of_players <= 0) {
    return;
  }

  player_list_free(*player_list);
  player_list_init(player_list);

  for (int i = 0; i < number_of_players; i++) {
    player_list_insert(player_list, players[i]);
  }
}

/* FIXME: refactor this mess. */
void player_list_insert(struct player_list **player_list, struct player player) {
  struct player_list **iterator = player_list;
  struct player_list *tmp = *iterator;
  struct player_list *previous, *node;

  if (*iterator) {
    while (tmp) {
      previous = tmp;
      tmp = tmp->next;
    }
    player_list_malloc(&node);
    player_malloc(&(node->player));
    player_cpy(&(node->player), &player);
    node->next = NULL;
    previous->next = node;
  } else {
    player_list_malloc(iterator);
    player_malloc(&((*iterator)->player));
    player_cpy(&((*iterator)->player), &player);
    (*iterator)->next = NULL;
  }
}

const struct player_list *player_list_find_by_player_id(struct player_list *player_list, int player_id) {
  for (; player_list; player_list = player_list->next) {
    if (player_list->player && player_list->player->id == player_id) {
      return(player_list);
    }
  }
  return(NULL);
}
