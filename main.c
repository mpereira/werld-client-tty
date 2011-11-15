#include <curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "client.h"
#include "keyboard.h"
#include "message_bar.h"
#include "player.h"
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
  cbreak();
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

  player_malloc(&(werld_client.player));
  player_set(werld_client.player, 0, name, 0, 0);

  if (client_connect(*(werld_client.player)) == -1) {
    endwin();
    werld_client_log(WERLD_CLIENT_INFO,
                     "%s: failed to connect to the server\n",
                     argv[0]);
    return(-1);
  }

  if (client_handle_response() == -1) {
    client_disconnect(*(werld_client.player));
    endwin();
    werld_client_log(WERLD_CLIENT_INFO,
                     "%s: connection to the server has been lost\n",
                     argv[0]);
    return(-1);
  }

  player_list_malloc(&(werld_client.player_list));
  werld_client.player_list->player = werld_client.player;
  werld_client.player_list->message_list = NULL;
  werld_client.player_list->next = NULL;

  fd_set master_fds, read_fds;

  FD_ZERO(&master_fds);
  FD_SET(fileno(stdin), &master_fds);
  FD_SET(werld_client.fd, &master_fds);

  do {
    read_fds = master_fds;
    if (select(werld_client.fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      continue;
    }
    if (FD_ISSET(fileno(stdin), &read_fds)) {
      keyboard_event(wgetch(stdscr));
    } else if (FD_ISSET(werld_client.fd, &read_fds)) {
      if (client_handle_response() == -1) {
        client_disconnect(*(werld_client.player));
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
