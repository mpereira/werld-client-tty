#include "player_list.h"

void player_list_free(struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    if (player_list->player) {
      player_free(player_list->player);
    }
  }
}
