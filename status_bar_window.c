#include <curses.h>

#include "player.h"
#include "ui.h"

static const int STATUS_BAR_WINDOW_LINES = 1;
static const int STATUS_BAR_WINDOW_COLUMNS = 80;
static const int STATUS_BAR_WINDOW_STARTING_Y = 23;
static const int STATUS_BAR_WINDOW_STARTING_X = 0;

void status_bar_window_new(WINDOW **status_bar_window) {
  *status_bar_window = newwin(STATUS_BAR_WINDOW_LINES,
                              STATUS_BAR_WINDOW_COLUMNS,
                              STATUS_BAR_WINDOW_STARTING_Y,
                              STATUS_BAR_WINDOW_STARTING_X);
}

void status_bar_window_del(WINDOW *status_bar_window) {
  delwin(status_bar_window);
}

void status_bar_window_refresh(WINDOW *status_bar_window, struct player *player) {
  werase(status_bar_window);

  waddstr(status_bar_window, player->name);
  /* FIXME: switch fixture data for real player stats. */
  waddstr(status_bar_window, " the Human\t\t");

  wattron(status_bar_window,  COLOR_PAIR(BLUE_ON_BLACK) | A_DIM);
  waddstr(status_bar_window, "Lvl");
  wattroff(status_bar_window,  COLOR_PAIR(BLUE_ON_BLACK) | A_DIM);
  wprintw(status_bar_window, ":%d ", 1);

  waddch(status_bar_window, '$' | COLOR_PAIR(YELLOW_ON_BLACK) | A_DIM);
  wprintw(status_bar_window, ":%d ", 1);

  wattron(status_bar_window,  COLOR_PAIR(RED_ON_BLACK) | A_DIM);
  waddstr(status_bar_window, "HP");
  wattroff(status_bar_window,  COLOR_PAIR(RED_ON_BLACK) | A_DIM);
  wprintw(status_bar_window, ":%d(%d) ", 10, 10);

  wattron(status_bar_window,  COLOR_PAIR(GREEN_ON_BLACK) | A_DIM);
  waddstr(status_bar_window, "Exp");
  wattroff(status_bar_window,  COLOR_PAIR(GREEN_ON_BLACK) | A_DIM);
  wprintw(status_bar_window, ":%d", 1);

  wrefresh(status_bar_window);
}
