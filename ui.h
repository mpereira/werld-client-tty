#ifndef WERLD_UI_H
#define WERLD_UI_H

#include "message_list.h"
#include "player.h"
#include "player_list.h"

void ui_draw_player(struct player);
void ui_erase_player(struct player);
void ui_draw_player_list(const struct player_list *);
void ui_erase_player_list(const struct player_list *);
void ui_draw_player_message_list(const struct player *);
void ui_erase_player_message_list(const struct player *);

#endif
