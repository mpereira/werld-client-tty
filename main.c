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

int main(int argc, const char *argv[]) {
  char *name;
  char buffer[MAX_NAME_SIZE];

  if (!(name = malloc(MAX_NAME_SIZE))) {
    fprintf(stderr, "%s\n", strerror(errno));
    exit(errno);
  }

  initscr();
  raw();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  clear();
  refresh();

  printw("What's your name? ");
  refresh();
  wgetnstr(stdscr, buffer, sizeof(buffer));
  sscanf(buffer, "%s", name);

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

  do {
    keyboard_event(getch());
  } while (true);

  return(0);
}
