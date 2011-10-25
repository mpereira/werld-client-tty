#include <curses.h>
#include <stdbool.h>
#include <string.h>

#include "keyboard.h"
#include "player.h"
#include "ui.h"
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

  struct player_list *player_list = werld_client_get_players();
  ui_draw_player_list(player_list);
  player_list_free(player_list);

  do {
    keyboard_event(getch());
    /* Hack 'til we handle events from the server. */
    player_list = werld_client_get_players();
    ui_draw_player_list(player_list);
    player_list_free(player_list);
  } while (true);

  return(0);
}
