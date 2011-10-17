#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "keyboard.h"
#include "movement.h"
#include "player.h"
#include "werld_client.h"

extern struct player player;

void keyboard_event(int key) {
  if (key == 'q' || key == 'Q') {
    werld_client_disconnect();
    endwin();
    exit(0);
  }
  switch (key) {
  case 'h':
  case 'j':
  case 'k':
  case 'l':
  case KEY_LEFT:
  case KEY_DOWN:
  case KEY_UP:
  case KEY_RIGHT:
    player_move(&player, movement_direction(key));
    break;
  default:
    break;
  }
}
