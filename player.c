#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "main_window.h"
#include "movement.h"
#include "player.h"
#include "tile.h"
#include "ui.h"
#include "werld_client.h"

void player_malloc(struct player **player) {
  if (!(*player = malloc(sizeof(struct player)))) {
    perror("malloc");
    exit(errno);
  }
}

void player_free(struct player **player) {
  free(*player);
  *player = NULL;
}

void player_set(struct player *player,
                int id,
                const char *name,
                uint32_t y,
                uint32_t x) {
  player->id = id;
  strncpy(player->name, name, strlen(name));
  player->y = y;
  player->x = x;
}

static bool player_can_traverse(struct player *player, enum movement movement) {
  struct tile tile;

  switch (movement) {
  case LEFT:
    tile = werld_client.world_map->tiles[player->x - 1][player->y];
    break;
  case DOWN:
    tile = werld_client.world_map->tiles[player->x][player->y + 1];
    break;
  case UP:
    tile = werld_client.world_map->tiles[player->x][player->y - 1];
    break;
  case RIGHT:
    tile = werld_client.world_map->tiles[player->x + 1][player->y];
    break;
  default:
    assert(false && "invalid movement");
  }

  return(tile.properties & WERLD_TILE_PROPERTY_TRAVERSABLE);
}

int player_move(struct player *player, enum movement movement) {
  if (!player_can_traverse(player, movement)) return(-1);

  ui_draw_map(werld_client.world_map);
  ui_erase_player(*player);
  ui_erase_player_message_list(werld_client.player);
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
  ui_draw_player_message_list(werld_client.player);
  main_window_refresh(werld_client.main_window);

  return(0);
}
