#include <curses.h>
#include <stdio.h>
#include <string.h>

#include "player.h"

void ui_draw_player(struct player player) {
  mvprintw(player.y - 1,
           player.x - strlen(player.name) / 2,
           "(%s)",
           player.name);
  mvprintw(player.y, player.x, "@");
}

void ui_erase_player(struct player player) {
  clear();
  refresh();
}
