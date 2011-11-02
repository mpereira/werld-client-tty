#define _POSIX_SOURCE
#include <curses.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

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

  if (werld_client_connect(player) == -1) {
    endwin();
    fprintf(stderr, "%s: failed to connect to the server\n", argv[0]);
    return(-1);
  }

  if (werld_client_handle_response() == -1) {
    endwin();
    fprintf(stderr, "%s: connection to the server has been lost\n", argv[0]);
    return(-1);
  }

  player_list_init(&player_list);
  player_list_insert(&player_list, player);

  fd_set master_fds, read_fds;

  FD_ZERO(&master_fds);
  FD_SET(fileno(stdin), &master_fds);
  FD_SET(fd, &master_fds);

  do {
    read_fds = master_fds;
    if (select(fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      continue;
    }
    if (FD_ISSET(fileno(stdin), &read_fds)) {
      keyboard_event(wgetch(stdscr));
    } else if (FD_ISSET(fd, &read_fds)) {
      if (werld_client_handle_response() == -1) {
        player_list_free(player_list);
        endwin();
        fprintf(stderr, "%s: connection to the server has been lost\n", argv[0]);
        return(-1);
      }
    }
  } while (true);

  return(0);
}
