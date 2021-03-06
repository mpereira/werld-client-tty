#ifndef WERLD_UI_H
#define WERLD_UI_H

#include "map.h"
#include "message_list.h"
#include "player.h"
#include "player_list.h"

#define BLACK_ON_BLACK   1
#define RED_ON_BLACK     2
#define GREEN_ON_BLACK   3
#define YELLOW_ON_BLACK  4
#define BLUE_ON_BLACK    5
#define MAGENTA_ON_BLACK 6
#define CYAN_ON_BLACK    7
#define WHITE_ON_BLACK   8

#define WERLD_TILE_TYPE_DIRT_COLOR  (COLOR_PAIR(YELLOW_ON_BLACK) | A_DIM)
#define WERLD_TILE_TYPE_GRASS_COLOR (COLOR_PAIR(GREEN_ON_BLACK)  | A_DIM)
#define WERLD_TILE_TYPE_SAND_COLOR  (COLOR_PAIR(YELLOW_ON_BLACK) | A_BOLD)
#define WERLD_TILE_TYPE_SNOW_COLOR  (COLOR_PAIR(WHITE_ON_BLACK)  | A_DIM)
#define WERLD_TILE_TYPE_WATER_COLOR (COLOR_PAIR(BLUE_ON_BLACK)   | A_DIM)
#define WERLD_TILE_TYPE_STONE_COLOR (COLOR_PAIR(BLACK_ON_BLACK)  | A_BOLD)
#define WERLD_TILE_TYPE_WALL_COLOR  (COLOR_PAIR(BLACK_ON_BLACK)  | A_BOLD)

extern char WERLD_TILE_TYPE_CHARACTER[];
extern int WERLD_TILE_TYPE_COLOR[];

void ui_draw_player(struct player);
void ui_erase_player(struct player);
void ui_draw_player_list(const struct player_list *);
void ui_erase_player_list(const struct player_list *);
void ui_draw_player_message_list(const struct player *);
void ui_erase_player_message_list(const struct player *);
void ui_draw_map(const struct map *);

#endif
