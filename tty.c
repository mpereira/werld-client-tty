#include <curses.h>

#include "ui.h"
#include "tty.h"

bool tty_term_size_ok() {
  int lines, columns;
  getmaxyx(stdscr, lines, columns);
  return(lines >= WERLD_MIN_TERM_LINES && columns >= WERLD_MIN_TERM_COLS);
}

void tty_handle_resize(void) {
  clear();
  if (tty_term_size_ok()) {
    ui_draw_player_list(player_list);
  } else {
    mvaddstr(1, 1, WERLD_SMALL_TERM_MSG);
  }
  refresh();
}
