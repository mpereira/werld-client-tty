#ifndef WERLD_MESSAGE_BAR_H
#define WERLD_MESSAGE_BAR_H

#include <curses.h>

void message_bar_new(WINDOW **);
void message_bar_del(WINDOW *);
void message_bar_getstr(WINDOW *, char *);

#endif
