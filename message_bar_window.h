#ifndef WERLD_MESSAGE_BAR_WINDOW_H
#define WERLD_MESSAGE_BAR_WINDOW_H

#include <curses.h>

void message_bar_window_new(WINDOW **);
void message_bar_window_init(WINDOW *);
void message_bar_window_del(WINDOW *);
void message_bar_window_getstr(WINDOW *, char *);

#endif
