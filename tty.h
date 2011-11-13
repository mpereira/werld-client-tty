#ifndef WERLD_TTY_H
#define WERLD_TTY_H

#include <curses.h>

#define WERLD_MIN_TERM_LINES 25
#define WERLD_MIN_TERM_COLS 80
#define WERLD_SMALL_TERM_MSG "Please increase your terminal size to at least 80x25 or press q to quit."

bool tty_term_size_ok(void);
void tty_handle_resize(void);

#endif
