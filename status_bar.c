#include <curses.h>

#include "status_bar.h"

int _status_bar_columns(void) {
  int y, x;
  getmaxyx(stdscr, y, x);
  return(x);
}

int _status_bar_y(void) {
  int y, x;
  getmaxyx(stdscr, y, x);
  return(y - 1);
}

void status_bar_init(WINDOW **status_bar_window) {
  *status_bar_window = newwin(STATUS_BAR_LINES,
                              STATUS_BAR_COLUMNS,
                              STATUS_BAR_STARTING_Y,
                              STATUS_BAR_STARTING_X);
}

void status_bar_refresh(WINDOW *status_bar_window, struct player *player) {
  werase(status_bar_window);
  waddstr(status_bar_window, player->name);
  /* FIXME: switch fixture data for real player stats. */
  waddstr(status_bar_window, " the Human");
  waddstr(status_bar_window, "\tLvl:1  $:0  HP:10(10)  Exp:1");
  wrefresh(status_bar_window);
}
