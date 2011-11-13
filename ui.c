#include <curses.h>
#include <stdio.h>
#include <string.h>

#include "message_list.h"
#include "player.h"
#include "player_list.h"
#include "player_message_list.h"
#include "werld_client.h"

void ui_draw_player(struct player player) {
  move(player.y - 1, (player.x - strlen(player.name) / 2) -1);
  addch('(');
  addstr(player.name);
  addch(')');
  mvaddch(player.y, player.x, '@');
}

void ui_erase_player(struct player player) {
  move(player.y - 1, (player.x - strlen(player.name) / 2) -1);
  for (unsigned int i = 0; i < strlen(player.name) + 2; i++) {
    addch(' ');
  }
  move(player.y, player.x);
  addch(' ');
}

void ui_redraw_player(struct player player) {
  ui_erase_player(player);
  ui_draw_player(player);
}

void ui_draw_player_list(const struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    ui_draw_player(*(player_list->player));
  }
}

void ui_erase_player_list(const struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    ui_erase_player(*(player_list->player));
  }
}

void ui_redraw_player_list(const struct player_list *player_list) {
  for (; player_list; player_list = player_list->next) {
    ui_redraw_player(*(player_list->player));
  }
}

void ui_draw_player_with_message(struct player player, const char *message) {
  move(player.y - 3, (player.x - strlen(message) / 2));
  addstr(message);
  ui_draw_player(player);
}

void ui_draw_player_message_list(const struct player_message_list *player_message_list) {
  int player_id;
  int y, x;
  struct message_list *iterator;
  struct player *player;
  struct player_list *player_list;

  iterator = player_message_list ? player_message_list->message_list : NULL;

  if (player_message_list) {
    player_list = player_list_find_by_player_id(werld_client.player_list, player_message_list->player_id);
    player = player_list ? player_list->player : NULL;
    y = player->y;
    x = player->y;
  }

  for (; iterator; iterator = iterator->next) {
    if (player) {
      mvaddstr(y - 3, (x - strlen(iterator->message)) / 2, iterator->message);
      y--;
    }
  }
}

void ui_erase_player_message_list(const struct player_message_list *player_message_list) {
  int player_id;
  int y, x;
  struct message_list *iterator;
  struct player *player;
  struct player_list *player_list;

  iterator = player_message_list ? player_message_list->message_list : NULL;

  if (player_message_list) {
    player_list = player_list_find_by_player_id(werld_client.player_list, player_message_list->player_id);
    player = player_list ? player_list->player : NULL;
    y = player->y;
    x = player->y;
  }

  for (; iterator; iterator = iterator->next) {
    if (player) {
      move(y - 3, (x - strlen(iterator->message)) / 2);
      for (unsigned int i = 0; i < strlen(iterator->message); i++) {
        addch(' ');
      }
      y--;
    }
  }
}

void ui_draw_player_message(struct player player, const char *message) {
  mvprintw(player.y - 3,
           (player.x - strlen(message) / 2),
           "%s",
           message);
}
