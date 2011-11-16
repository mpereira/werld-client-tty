#ifndef WERLD_MESSAGE_BAR_H
#define WERLD_MESSAGE_BAR_H

#include <curses.h>

static const int MESSAGE_BAR_LINES = 1;
#define MESSAGE_BAR_COLUMNS _message_bar_columns()
#define MESSAGE_BAR_STARTING_Y _message_bar_y()
static const int MESSAGE_BAR_STARTING_X = 0;

int _message_bar_columns(void);
int _message_bar_y(void);
void message_bar_getstr(WINDOW *, char *);
void message_bar_init(WINDOW **);

#endif
