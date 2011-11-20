#ifndef WERLD_STATUS_BAR_H
#define WERLD_STATUS_BAR_H

#include <curses.h>

#include "player.h"

static const int STATUS_BAR_LINES = 1;
#define STATUS_BAR_COLUMNS _status_bar_columns()
#define STATUS_BAR_STARTING_Y _status_bar_y()
static const int STATUS_BAR_STARTING_X = 0;

int _status_bar_columns(void);
int _status_bar_y(void);
void status_bar_init(WINDOW **);
void status_bar_refresh(WINDOW *, struct player *);

#endif
