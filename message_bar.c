#include <curses.h>

#include "message.h"
#include "message_bar.h"

static const int MESSAGE_BAR_LINES = 1;
static const int MESSAGE_BAR_COLUMNS = 80;
static const int MESSAGE_BAR_STARTING_Y = 22;
static const int MESSAGE_BAR_STARTING_X = 0;

void message_bar_new(WINDOW **message_bar_window) {
  *message_bar_window = newwin(MESSAGE_BAR_LINES,
                               MESSAGE_BAR_COLUMNS,
                               MESSAGE_BAR_STARTING_Y,
                               MESSAGE_BAR_STARTING_X);
}

void message_bar_init(WINDOW *window) {
  keypad(window, true);
}

void message_bar_del(WINDOW *message_bar_window) {
  delwin(message_bar_window);
}

void message_bar_getstr(WINDOW *message_bar_window, char *message) {
  echo();
  curs_set(true);
  mvwaddch(message_bar_window, 0, 0, '>');
  mvwgetnstr(message_bar_window, 0, 2, message, WERLD_PLAYER_MESSAGE_MAX_LENGTH);
  werase(message_bar_window);
  wrefresh(message_bar_window);
  curs_set(false);
  noecho();
}
