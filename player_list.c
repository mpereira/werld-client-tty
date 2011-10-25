#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "player_list.h"

void player_list_init(struct player_list **player_list) {
  *player_list = NULL;
}

void player_list_free(struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    if (player_list->player) {
      player_free(player_list->player);
    }
  }
}

void player_list_fill(struct player_list **player_list,
                      const struct player players[],
                      const int number_of_players) {
  struct player_list *iterator;

  if (!(*player_list = malloc(sizeof(**player_list)))) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  iterator = *player_list;

  for (int i = 0; i < number_of_players; i++) {
    if (!(iterator->player = malloc(sizeof(struct player)))) {
      fprintf(stderr, strerror(errno));
      exit(errno);
    }

    *(iterator->player) = players[i];

    if (i == number_of_players - 1) {
      iterator->next = NULL;
    } else {
      if (!(iterator->next = malloc(sizeof(struct player_list)))) {
        fprintf(stderr, strerror(errno));
        exit(errno);
      }
      iterator = iterator->next;
    }
  }
}
