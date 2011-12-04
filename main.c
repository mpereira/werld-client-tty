#include <curses.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "client.h"
#include "keyboard.h"
#include "maps.h"
#include "message_bar.h"
#include "message_handler.h"
#include "player.h"
#include "player_list.h"
#include "status_bar.h"
#include "tty.h"
#include "ui.h"
#include "werld_client.h"
#include "window.h"

int main(int argc, const char *argv[]) {
  char *account;
  char *password;
  int key;
  struct timeval timeout;

  if (!(account = malloc(WERLD_ACCOUNT_MAX_SIZE))) {
    perror("malloc");
    exit(errno);
  }

  if (!(password = malloc(WERLD_PASSWORD_MAX_SIZE))) {
    perror("malloc");
    exit(errno);
  }

  /* FIXME: this removes a stupid warning. */
  (void) argc;

  werld_client_init(&werld_client);

  /* FIXME: parse config options from command-line arguments. */
  werld_client.log_level = WERLD_CLIENT_DEBUG;
  werld_client.log_file = NULL;
  werld_client.player_messages_lifetime = 3;

  while (!tty_term_size_ok()) {
    clear();
    mvaddstr(1, 1, WERLD_SMALL_TERM_MSG);
    if ((key = getch()) == 'q' || key == 'Q') {
      free(account);
      free(password);
      endwin();
      return(0);
    }
    refresh();
  }

  window_new(&(werld_client.window));
  window_init(werld_client.window);
  window_get_credentials(werld_client.window, account, password);

  player_malloc(&(werld_client.player));
  player_set(werld_client.player, 0, account, 0, 0);

  free(account);
  free(password);

  if (client_request_connect(*(werld_client.player)) == -1) {
    werld_client_kill(&werld_client);
    werld_client_log(WERLD_CLIENT_ERROR,
                     "%s: failed to connect to the server\n",
                     argv[0]);
    return(-1);
  }

  if (client_handle_response() == -1) {
    werld_client_kill(&werld_client);
    werld_client_log(WERLD_CLIENT_ERROR,
                     "%s: connection to the server has been lost\n",
                     argv[0]);
    return(-1);
  }

  if (client_request_map(WERLD_MAPS_WORLD) == -1) {
    werld_client_kill(&werld_client);
    werld_client_log(WERLD_CLIENT_ERROR,
                     "%s: failed to connect to the server\n",
                     argv[0]);
    return(-1);
  }

  if (client_handle_response() == -1) {
    werld_client_kill(&werld_client);
    werld_client_log(WERLD_CLIENT_ERROR,
                     "%s: failed to download world map\n",
                     argv[0]);
    return(-1);
  }

  if (pipe(werld_client.message_handler_fds) == -1) {
    perror("pipe");
    return(errno);
  }

  player_list_malloc(&(werld_client.player_list));
  werld_client.player_list->player = werld_client.player;
  werld_client.player_list->message_list = NULL;
  werld_client.player_list->next = NULL;

  message_bar_new(&(werld_client.message_bar));
  message_bar_init(werld_client.message_bar);
  status_bar_new(&(werld_client.status_bar));

  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    wbkgd(werld_client.status_bar, COLOR_PAIR(1));
  }
  status_bar_refresh(werld_client.status_bar, werld_client.player);
  ui_draw_map(werld_client.world_map);
  window_refresh(werld_client.window);

  fd_set master_fds, read_fds;

  FD_ZERO(&master_fds);
  FD_SET(fileno(stdin), &master_fds);
  FD_SET(werld_client.fd, &master_fds);
  FD_SET(werld_client.message_handler_fds[0], &master_fds);

  for (;;) {
    read_fds = master_fds;
    timeout.tv_sec = werld_client.player_messages_lifetime;
    timeout.tv_usec = 0;

    if (select(werld_client.message_handler_fds[0] + 1,
               &read_fds,
               NULL,
               NULL,
               &timeout) == -1) {
      perror("select");
      continue;
    }
    if (FD_ISSET(fileno(stdin), &read_fds)) {
      keyboard_event(wgetch(werld_client.window));
    }
    if (FD_ISSET(werld_client.message_handler_fds[0], &read_fds)) {
      if (message_handler_handle_player_message() == -1) {
        /* code */
      }
    }
    if (FD_ISSET(werld_client.fd, &read_fds)) {
      if (client_handle_response() == -1) {
        werld_client_kill(&werld_client);
        werld_client_log(WERLD_CLIENT_INFO,
                         "%s: connection to the server has been lost\n",
                         argv[0]);
        return(-1);
      }
    }
    message_handler_sweep_messages();
  }

  return(0);
}
