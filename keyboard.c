#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "message_bar.h"
#include "movement.h"
#include "player.h"
#include "player_list.h"
#include "werld_client.h"

void keyboard_event(int key) {
  char message[WERLD_PLAYER_MESSAGE_BUFSIZ];

  switch (key) {
  case 'q':
    client_disconnect(player);
    player_list_free(player_list);
    endwin();
    exit(0);
  case '\n':
    /* FIXME: make this asynchronous. */
    message_bar_getstr(werld_client.message_bar, message);
    client_send_message(player, message);
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
