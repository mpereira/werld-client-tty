#ifndef WERLD_CLIENT_UI_H
#define WERLD_CLIENT_UI_H

#include "player.h"
#include "player_list.h"

void ui_draw_player(const struct player);
void ui_erase_player(const struct player);
void ui_redraw_player(const struct player);
void ui_draw_player_list(const struct player_list *);

#endif
