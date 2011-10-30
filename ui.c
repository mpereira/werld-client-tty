#include <curses.h>
#include <stdio.h>
#include <string.h>

#include "player.h"
#include "player_list.h"

void ui_draw_player(const struct player player) {
  mvprintw(player.y - 1,
           player.x - strlen(player.name) / 2,
           "(%s)",
           player.name);
  mvprintw(player.y, player.x, "@");
  refresh();
}

void ui_erase_player(const struct player player) {
  clear();
  refresh();
}

void ui_redraw_player(const struct player player) {
  ui_erase_player(player);
  ui_draw_player(player);
}

void ui_draw_player_list(const struct player_list *player_list) {
  clear();
  for (; player_list; player_list = player_list->next) {
    ui_draw_player(*(player_list->player));
  }
}
