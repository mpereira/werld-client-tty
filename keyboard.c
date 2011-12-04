#include <curses.h>
#include <errno.h>
#include <stdlib.h>

#include "client.h"
#include "message.h"
#include "message_bar.h"
#include "movement.h"
#include "player.h"
#include "tty.h"
#include "ui.h"
#include "werld_client.h"
#include "window.h"

void keyboard_event(int key) {
  char *message;

  if (key == 'q' || key == 'Q') {
    werld_client_kill(&werld_client);
    exit(0);
  }
  if (tty_term_size_ok()) {
    switch (key) {
    case '\n':
      if (!(message = malloc(WERLD_PLAYER_MESSAGE_MAX_LENGTH + 1))) {
        perror("malloc");
        exit(errno);
      }
      /* FIXME: make this asynchronous. */
      message_bar_getstr(werld_client.message_bar, message);
      client_request_message(*(werld_client.player), message);
      free(message);
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
      ui_draw_map(werld_client.world_map);
      player_move(werld_client.player, movement_direction(key));
      ui_draw_player_message_list(werld_client.player);
      window_refresh(werld_client.window);
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
