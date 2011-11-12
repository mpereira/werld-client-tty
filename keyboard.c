#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "movement.h"
#include "player.h"
#include "player_list.h"

#define WERLD_MESSAGE_INPUT_Y 22
#define WERLD_MESSAGE_INPUT_X 0

void keyboard_event(int key) {
  char message[WERLD_PLAYER_MESSAGE_BUFSIZ];

  switch (key) {
  case 'q':
    client_disconnect(player);
    player_list_free(player_list);
    endwin();
    exit(0);
  case '\n':
    echo();
    curs_set(true);
    mvwgetnstr(stdscr,
               WERLD_MESSAGE_INPUT_Y,
               WERLD_MESSAGE_INPUT_X,
               message,
               sizeof(message) - 1);
    client_send_message(player, message);
    curs_set(false);
    noecho();
    break;
  case 'h':
  case 'j':
  case 'k':
  case 'l':
  case KEY_LEFT:
  case KEY_DOWN:
  case KEY_UP:
  case KEY_RIGHT:
    player_move(&player, movement_direction(key));
    refresh();
    break;
  default:
    break;
  }
}
