#include <curses.h>
#include <stdlib.h>

#include "client.h"
#include "message_bar.h"
#include "message_bar.h"
#include "movement.h"
#include "player.h"
#include "status_bar.h"
#include "tty.h"
#include "ui.h"
#include "werld_client.h"
#include "window.h"

void keyboard_event(int key) {
  char message[WERLD_PLAYER_MESSAGE_BUFSIZ];

  if (key == 'q' || key == 'Q') {
    client_disconnect(*(werld_client.player));
    player_list_free(werld_client.player_list);
    message_bar_del(werld_client.message_bar);
    status_bar_del(werld_client.status_bar);
    window_del(werld_client.status_bar);
    endwin();
    exit(0);
  }
  if (tty_term_size_ok()) {
    switch (key) {
    case '\n':
      /* FIXME: make this asynchronous. */
      message_bar_getstr(werld_client.message_bar, message);
      client_send_message(*(werld_client.player), message);
      break;
    case 'h':
    case 'j':
    case 'k':
    case 'l':
    case KEY_LEFT:
    case KEY_DOWN:
    case KEY_UP:
    case KEY_RIGHT:
      ui_erase_player_message_list(werld_client.player);
      player_move(werld_client.player, movement_direction(key));
      ui_draw_player_message_list(werld_client.player);
      refresh();
      break;
    case KEY_RESIZE:
      tty_handle_resize();
      break;
    default:
      break;
    }
  } else {
    tty_handle_resize();
  }
}
