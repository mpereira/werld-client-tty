#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "keyboard.h"
#include "message_bar.h"
#include "player.h"
#include "ui.h"
#include "tty.h"
#include "werld_client.h"

int main(int argc, const char *argv[]) {
  char name[WERLD_PLAYER_NAME_BUFSIZ];
  int key;

  /* FIXME: parse config options from command-line arguments. */
  werld_client.log_level = WERLD_CLIENT_DEBUG;
  werld_client.log_file = NULL;
  werld_client.player_messages_lifetime = 3;

  initscr();
  raw();
  keypad(stdscr, true);
  curs_set(false);

  while (!tty_term_size_ok()) {
    clear();
    mvaddstr(1, 1, WERLD_SMALL_TERM_MSG);
    if ((key = getch()) == 'q' || key == 'Q') {
      endwin();
      return(0);
    }
    refresh();
  }

  /* FIXME: make this resistant to terminal resizing. */
  clear();
  mvaddstr(0, 0, "What's your name? ");
  refresh();
  wgetnstr(stdscr, name, sizeof(name));

  noecho();
  clear();
  refresh();

  message_bar_init(&(werld_client.message_bar));

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    wbkgd(werld_client.message_bar, COLOR_PAIR(1));
  }
  wrefresh(werld_client.message_bar);

  /*id = server_get_id();*/
  int id = 1;
  /*y = server_get_y();*/
  int y = 10;
  /*x = server_get_x();*/
  int x = 10;

  player_initialize(&player, id, name, y, x);

  if (client_connect(player) == -1) {
    endwin();
    werld_client_log(WERLD_CLIENT_INFO,
                     "%s: failed to connect to the server\n",
                     argv[0]);
    return(-1);
  }

  if (client_handle_response() == -1) {
    client_disconnect(player);
    endwin();
    werld_client_log(WERLD_CLIENT_INFO,
                     "%s: connection to the server has been lost\n",
                     argv[0]);
    return(-1);
  }

  player_list_init(&(werld_client.player_list));
  player_list_insert(&(werld_client.player_list), player);

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
      if (client_handle_response() == -1) {
        client_disconnect(player);
        player_list_free(werld_client.player_list);
        endwin();
        werld_client_log(WERLD_CLIENT_INFO,
                         "%s: connection to the server has been lost\n",
                         argv[0]);
        return(-1);
      }
    }
  } while (true);

  return(0);
}
