#include <curses.h>
#include <errno.h>
#include <stdlib.h>

#include "client.h"
#include "main_window.h"
#include "message.h"
#include "message_bar_window.h"
#include "movement.h"
#include "player.h"
#include "tty.h"
#include "werld_client.h"

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
      message_bar_window_getstr(werld_client.message_bar_window, message);
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
      if (player_move(werld_client.player, movement_direction(key)) != -1) {
        client_request_player(*(werld_client.player));
      }
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
