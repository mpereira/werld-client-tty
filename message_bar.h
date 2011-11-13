#ifndef WERLD_MESSAGE_BAR_H
#define WERLD_MESSAGE_BAR_H

#include <curses.h>

#define MESSAGE_BAR_LINES 1
#define MESSAGE_BAR_COLUMNS _message_bar_columns()
#define MESSAGE_BAR_STARTING_Y _message_bar_y()
#define MESSAGE_BAR_STARTING_X 0

void message_bar_getstr(WINDOW *, char *);
void message_bar_init(WINDOW **);

#endif
