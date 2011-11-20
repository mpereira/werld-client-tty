#ifndef WERLD_STATUS_BAR_H
#define WERLD_STATUS_BAR_H

#include <curses.h>

#include "player.h"

void status_bar_init(WINDOW **);
void status_bar_refresh(WINDOW *, struct player *);

#endif
