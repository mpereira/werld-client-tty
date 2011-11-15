#include <curses.h>
#include <string.h>

#include "player.h"
#include "player_list.h"

void ui_draw_player(struct player player) {
  move(player.y - 1, (player.x - strlen(player.name) / 2) -1);
  addch('(');
  addstr(player.name);
  addch(')');
  mvaddch(player.y, player.x, '@');
}

void ui_draw_player_with_message(struct player player, const char *message) {
  move(player.y - 3, (player.x - strlen(message) / 2));
  addnstr(message, strlen(message));
  ui_draw_player(player);
}

void ui_erase_player(struct player player) {
  move(player.y - 1, (player.x - strlen(player.name) / 2) -1);
  for (unsigned int i = 0; i < strlen(player.name) + 2; i++) {
    addch(' ');
  }
  move(player.y, player.x);
  addch(' ');
}

void ui_redraw_player(struct player player) {
  ui_erase_player(player);
  ui_draw_player(player);
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

void ui_redraw_player_list(const struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    ui_redraw_player(*(player_list->player));
  }
}
