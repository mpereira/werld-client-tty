#include <curses.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "message_handler.h"
#include "message_list.h"
#include "net.h"
#include "werld_client.h"
#include "ui.h"

#define WERLD_MESSAGE_HANDLER_WRITE_BUFSIZ(message) \
  (sizeof(size_t) + sizeof(struct player) + strlen(message))

#define WERLD_MESSAGE_HANDLER_READ_BUFSIZ(message_size) \
  (sizeof(struct player) + message_size)

int message_handler_handle_player_message(void) {
  char *data;
  char *message;
  size_t message_size;
  ssize_t bytes_read;
  struct player player;

  if ((bytes_read = net_read(werld_client.message_handler_fds[0],
                             &message_size,
                             sizeof(size_t))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+message_handler+handle_player_message read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          (uint8_t *) &message_size,
                          sizeof(size_t),
                          "+message_handler+handle_player_message bytes read: %zd ",
                          bytes_read);

  if (!(data = malloc(WERLD_MESSAGE_HANDLER_READ_BUFSIZ(message_size)))) {
    perror("malloc");
    exit(errno);
  }

  if ((bytes_read = net_read(werld_client.message_handler_fds[0],
                             data,
                             WERLD_MESSAGE_HANDLER_READ_BUFSIZ(message_size))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+message_handler+handle_player_message read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  if (!(message = malloc(message_size))) {
    perror("malloc");
    exit(errno);
  }

  memcpy(&player, data, sizeof(struct player));
  memcpy(message, data + sizeof(struct player), message_size);
  message[message_size] = '\0';

  ui_erase_player_message_list(&player);
  player_list_add_message(&(werld_client.player_list), message, player.id);
  ui_draw_player_message_list(&player);
  wrefresh(werld_client.window);
  free(message);
  free(data);

  return(0);
}

void message_handler_handle_incoming_message(const struct player *player,
                                             const char *message) {
  char *data;
  size_t message_size = strlen(message) + 1;
  ssize_t bytes_written;


  if (!(data = malloc(WERLD_MESSAGE_HANDLER_WRITE_BUFSIZ(message)))) {
    perror("malloc");
    exit(errno);
  }

  void *offset = mempcpy(data, &message_size, sizeof(size_t));
  offset = mempcpy(offset, player, sizeof(struct player));
  memcpy(offset, message, message_size);

  if ((bytes_written = net_write(werld_client.message_handler_fds[1],
                                 data,
                                 WERLD_MESSAGE_HANDLER_WRITE_BUFSIZ(message))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+message_handler+handle_incoming_message write failed\n");
    exit(-1);
  }


  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          (uint8_t *) data,
                          WERLD_MESSAGE_HANDLER_WRITE_BUFSIZ(message),
                          "+message_handler+handle_incoming_message bytes written: %d ",
                          bytes_written);

  free(data);
}

void message_handler_sweep_messages(void) {
  struct player_list *i = werld_client.player_list;
  struct message_list *j;
  time_t now;
  time(&now);
  /* FIXME: take into account that the message list is actually a queue. */
  for (; i; i = i->next) {
    j = i->message_list;
    for (; j; j = j->next) {
      if (difftime(now, j->created_at) > werld_client.player_messages_lifetime) {
        werld_client_log(WERLD_CLIENT_DEBUG,
                         "+message_handler_sweep_messages sweeping %s\n",
                         j->message);
        ui_erase_player_message_list(i->player);
        message_list_remove(&(i->message_list), j->message);
        ui_draw_player_list(werld_client.player_list);
        ui_draw_player_message_list(i->player);
        wrefresh(werld_client.window);
      }
    }
  }
}
