#include <curses.h>
#include <string.h>

#include "map.h"
#include "message_list.h"
#include "player.h"
#include "player_list.h"
#include "ui.h"
#include "werld_client.h"

char WERLD_TILE_TYPE_CHARACTER[] = {
  '.',  /* WERLD_TILE_TYPE_DIRT  */
  '.',  /* WERLD_TILE_TYPE_GRASS */
  '.',  /* WERLD_TILE_TYPE_SAND  */
  '.',  /* WERLD_TILE_TYPE_SNOW  */
  '.',  /* WERLD_TILE_TYPE_WATER */
  '.',  /* WERLD_TILE_TYPE_STONE */
  '#'   /* WERLD_TILE_TYPE_WALL  */
};

int WERLD_TILE_TYPE_COLOR[] = {
  WERLD_TILE_TYPE_DIRT_COLOR,
  WERLD_TILE_TYPE_GRASS_COLOR,
  WERLD_TILE_TYPE_SAND_COLOR,
  WERLD_TILE_TYPE_SNOW_COLOR,
  WERLD_TILE_TYPE_WATER_COLOR,
  WERLD_TILE_TYPE_STONE_COLOR,
  WERLD_TILE_TYPE_WALL_COLOR
};

void ui_draw_player(struct player player) {
  wmove(werld_client.main_window, player.y - 1, (player.x - strlen(player.name) / 2) -1);
  waddch(werld_client.main_window, '(' | COLOR_PAIR(BLACK_ON_BLACK) | A_BOLD);
  wattron(werld_client.main_window,  COLOR_PAIR(CYAN_ON_BLACK) | A_DIM);
  waddstr(werld_client.main_window, player.name);
  wattroff(werld_client.main_window,  COLOR_PAIR(CYAN_ON_BLACK) | A_DIM);
  waddch(werld_client.main_window, ')' | COLOR_PAIR(BLACK_ON_BLACK) | A_BOLD);
  mvwaddch(werld_client.main_window, player.y, player.x, '@');
}

void ui_erase_player(struct player player) {
  wmove(werld_client.main_window, player.y - 1, (player.x - strlen(player.name) / 2) -1);
  for (unsigned int i = 0; i < strlen(player.name) + 2; i++) {
    waddch(werld_client.main_window, ' ');
  }
  wmove(werld_client.main_window, player.y, player.x);
  waddch(werld_client.main_window, ' ');
}

void ui_draw_player_list(const struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    ui_draw_player(*(player_list->player));
  }
}

void ui_erase_player_list(const struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    ui_erase_player(*(player_list->player));
  }
}

/* FIXME: refactor and don't depend on werld_client.player_list. */
void ui_draw_player_message_list(const struct player *player) {
  int y, x;
  struct message_list *message_list;
  struct player_list *player_list = werld_client.player_list;

  if (!player) return;

  message_list = NULL;
  for (; player_list; player_list = player_list->next) {
    if (player_list->player && player_list->player->id == player->id) {
      message_list = player_list->message_list;
    }
  }

  y = player->y;
  x = player->x;

  for (; message_list; message_list = message_list->next) {
    mvwaddstr(werld_client.main_window,
              y - 3,
              x - strlen(message_list->message) / 2,
              message_list->message);
    y--;
  }
}

/* FIXME: refactor and don't depend on werld_client.player_list. */
void ui_erase_player_message_list(const struct player *player) {
  int y, x;
  struct message_list *message_list;
  struct player_list *player_list = werld_client.player_list;

  if (!player) return;

  message_list = NULL;
  for (; player_list; player_list = player_list->next) {
    if (player_list->player && player_list->player->id == player->id) {
      message_list = player_list->message_list;
    }
  }

  y = player->y;
  x = player->x;

  for (; message_list; message_list = message_list->next) {
    wmove(werld_client.main_window,
          y - 3,
          x - strlen(message_list->message) / 2);
    for (size_t i = 0; i < strlen(message_list->message); i++) {
      waddch(werld_client.main_window, ' ');
    }
    y--;
  }
}

void ui_draw_map(const struct map *map) {
  for (unsigned int i = 0; i < map->width; i++) {
    for (unsigned int j = 0; j < map->height; j++) {
      mvwaddch(werld_client.main_window,
               j,
               i,
               WERLD_TILE_TYPE_CHARACTER[map->tiles[i][j].type] |
                 WERLD_TILE_TYPE_COLOR[map->tiles[i][j].type]);
    }
  }
}
