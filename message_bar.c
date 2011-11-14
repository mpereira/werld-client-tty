#include <curses.h>

#include "client.h"
#include "message_bar.h"

int _message_bar_columns(void) {
  int y, x;
  getmaxyx(stdscr, y, x);
  return(x);
}

int _message_bar_y(void) {
  int y, x;
  getmaxyx(stdscr, y, x);
  return(y - 2);
}

void message_bar_init(WINDOW **message_bar_window) {
  *message_bar_window = newwin(MESSAGE_BAR_LINES,
                               MESSAGE_BAR_COLUMNS,
                               MESSAGE_BAR_STARTING_Y,
                               MESSAGE_BAR_STARTING_X);
}

void message_bar_getstr(WINDOW *message_bar_window, char *message) {
  echo();
  curs_set(true);
  mvwgetnstr(message_bar_window, 0, 0, message, WERLD_PLAYER_MESSAGE_BUFSIZ);
  werase(message_bar_window);
  wrefresh(message_bar_window);
  curs_set(false);
  noecho();
}
