#ifndef WERLD_MESSAGE_BAR_H
#define WERLD_MESSAGE_BAR_H

#include <curses.h>

void message_bar_init(WINDOW **);
void message_bar_getstr(WINDOW *, char *);

#endif
