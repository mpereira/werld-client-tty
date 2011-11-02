#ifndef WERLD_UI_H
#define WERLD_UI_H

#include "player.h"
#include "player_list.h"

void ui_draw_player(struct player);
void ui_draw_player_with_message(struct player, const char *);
void ui_erase_player(struct player);
void ui_redraw_player(struct player);
void ui_draw_player_list(const struct player_list *);

#endif
