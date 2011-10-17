#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

#include "ui.h"
#include "player.h"
#include "keyboard.h"
#include "werld_client.h"

int main(int argc, const char *argv[]) {
  char name[MAX_NAME_SIZE];

  initscr();
  raw();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  clear();
  refresh();

  printw("What's your name? ");
  refresh();
  wgetnstr(stdscr, name, sizeof(name));

  noecho();
  clear();
  refresh();

  /*id = server_get_id();*/
  int id = 1;
  /*y = server_get_y();*/
  int y = 10;
  /*x = server_get_x();*/
  int x = 10;

  player_initialize(&player, id, name, y, x);
  ui_draw_player(player);

  werld_client_connect();

  endwin();
  werld_client_send_player(player);

  do {
    keyboard_event(getch());
  } while (true);

  endwin();
  werld_client_disconnect();

  return(0);
}
