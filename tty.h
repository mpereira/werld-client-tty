#ifndef WERLD_TTY_H
#define WERLD_TTY_H

#include <curses.h>

static const int WERLD_MIN_TERM_LINES = 25;
static const int WERLD_MIN_TERM_COLS = 80;
static const char *WERLD_SMALL_TERM_MSG = "Please increase your terminal size to at least 80x25 or press q to quit.";

bool tty_term_size_ok(void);
void tty_handle_resize(void);

#endif
