#include <curses.h>

#include "player.h"

static const int STATUS_BAR_LINES = 1;
static const int STATUS_BAR_COLUMNS = 80;
static const int STATUS_BAR_STARTING_Y = 23;
static const int STATUS_BAR_STARTING_X = 0;

void status_bar_new(WINDOW **status_bar_window) {
  *status_bar_window = newwin(STATUS_BAR_LINES,
                              STATUS_BAR_COLUMNS,
                              STATUS_BAR_STARTING_Y,
                              STATUS_BAR_STARTING_X);
}

void status_bar_del(WINDOW *status_bar_window) {
  delwin(status_bar_window);
}

void status_bar_refresh(WINDOW *status_bar_window, struct player *player) {
  werase(status_bar_window);
  waddstr(status_bar_window, player->name);
  /* FIXME: switch fixture data for real player stats. */
  waddstr(status_bar_window, " the Human");
  waddstr(status_bar_window, "\tLvl:1  $:0  HP:10(10)  Exp:1");
  wrefresh(status_bar_window);
}
