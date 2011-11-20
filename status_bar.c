#include <curses.h>

#include "status_bar.h"

static const int STATUS_BAR_LINES = 1;
#define STATUS_BAR_COLUMNS _status_bar_columns()
#define STATUS_BAR_STARTING_Y _status_bar_y()
static const int STATUS_BAR_STARTING_X = 0;

static int _status_bar_columns(void) {
  int y, x;
  (void) y;
  getmaxyx(stdscr, y, x);
  return(x);
}

static int _status_bar_y(void) {
  int y, x;
  (void) x;
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
