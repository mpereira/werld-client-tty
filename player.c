#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "movement.h"
#include "player.h"
#include "player_list.h"
#include "ui.h"
#include "werld_client.h"

void player_initialize(struct player *player,
                       const int id,
                       const char *name,
                       const int y,
                       const int x) {
  player->id = id;
  strncpy(player->name, name, strlen(name));
  player->y = y;
  player->x = x;
}

void player_free(struct player *player) {
  if (player) {
    free(player);
  }
}

void player_move(struct player *player, enum movement movement) {
  ui_erase_player(*player);
  switch (movement) {
  case LEFT:
    player->x = player->x - 1;
    break;
  case DOWN:
    player->y = player->y + 1;
    break;
  case UP:
    player->y = player->y - 1;
    break;
  case RIGHT:
    player->x = player->x + 1;
    break;
  }
  ui_draw_player(*player);
  werld_client_send_player(*player);
}
