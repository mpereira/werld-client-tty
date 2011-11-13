#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "message_handler.h"
#include "message_list.h"
#include "player_message_list.h"
#include "werld_client.h"
#include "ui.h"

int message_handler_handle_player_message(void) {
  char *message;
  char data[WERLD_MESSAGE_HANDLER_READ_BUFSIZ];
  size_t message_size;
  ssize_t bytes_read;
  struct player player;

  if ((bytes_read = read(werld_client.message_handler_fds[0],
                         data,
                         sizeof(data))) < 0) {
    perror("read");
    return(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          "+message_handler_init read %zd bytes ",
                          bytes_read);

  if (bytes_read == 0) {
    return(-1);
  }

  memcpy(&message_size, data, sizeof(size_t));
  if (!(message = malloc(message_size))) {
    perror("malloc");
    return(-1);
  }
  memcpy(&player, data + sizeof(size_t), sizeof(struct player));
  strncpy(message, data + sizeof(size_t) + sizeof(struct player), message_size);

  player_message_list_add(&(werld_client.player_message_list), message, player.id);
  free(message);

  return(0);
}

void message_handler_handle_incoming_message(const struct player *player,
                                             const char *message) {
  char data[WERLD_MESSAGE_HANDLER_WRITE_BUFSIZ(message)];
  size_t message_size = strlen(message) + 1;

  void *offset = mempcpy(data, &message_size, sizeof(size_t));
  offset = mempcpy(offset, player, sizeof(struct player));
  memcpy(offset, message, message_size);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          "+message_handler_handle_incoming_message writing %d bytes ",
                          sizeof(data));
  write(werld_client.message_handler_fds[1], data, sizeof(data));
}

void message_handler_sweep_messages(void) {
  struct player_message_list *i = werld_client.player_message_list;
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
        ui_erase_player_message_list(i);
        message_list_remove(&(i->message_list), j->message);
        ui_draw_player_list(werld_client.player_list);
        ui_draw_player_message_list(i);
        refresh();
      }
    }
  }
}
