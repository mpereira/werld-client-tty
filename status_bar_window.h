#ifndef WERLD_STATUS_BAR_WINDOW_H
#define WERLD_STATUS_BAR_WINDOW_H

#include <curses.h>

#include "player.h"

void status_bar_window_new(WINDOW **);
void status_bar_window_del(WINDOW *);
void status_bar_window_refresh(WINDOW *, struct player *);

#endif
