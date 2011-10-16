#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "movement.h"
#include "player.h"
#include "ui.h"

void player_initialize(struct player *player, int id, char *name, int y, int x) {
  if (!(player->name = malloc(strlen(name)))) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(errno);
  }
  strcpy(player->name, name);
  player->id = id;
  player->y = y;
  player->x = x;
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
}
