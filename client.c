#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "message_handler.h"
#include "net.h"
#include "player.h"
#include "player_list.h"
#include "request_type.h"
#include "ui.h"
#include "werld_client.h"

#ifdef WERLD_DEVELOPMENT
static const char *WERLD_SERVER_ADDRESS = "0.0.0.0";
#else
static const char *WERLD_SERVER_ADDRESS = "server.werldonline.com";
#endif
static const char *WERLD_SERVER_PORT = "9876";

enum { WERLD_RESPONSE_TYPE_ERROR = -1,
       WERLD_RESPONSE_TYPE_REGISTER,
       WERLD_RESPONSE_TYPE_PLAYERS,
       WERLD_RESPONSE_TYPE_MESSAGE };

static const size_t WERLD_RESPONSE_TYPE_BUFSIZ = 4;

#define WERLD_RESPONSE_REGISTER_BUFSIZ sizeof(struct player)
#define WERLD_RESPONSE_MESSAGE_BUFSIZ(message_length) (sizeof(struct player) + \
                                                       message_length)
#define WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players) (number_of_players * \
                                                          sizeof(struct player))

static void client_register(struct player player) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_REGISTER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  void *offset =
    mempcpy(data, &WERLD_REQUEST_TYPE_REGISTER, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(offset, &player, sizeof(struct player));

  if ((bytes_written = net_write(werld_client.fd,
                                 data,
                                 WERLD_REQUEST_TYPE_REGISTER_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+register write failed\n");
    exit(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_REGISTER_BUFSIZ,
                          "+client+register bytes written: %zd ",
                          bytes_written);
  free(data);
}

int client_connect(struct player player) {
  int keepalive;
  int status;
  struct addrinfo *iterator;
  struct addrinfo *results;
  struct addrinfo hints;
  struct timeval timeout;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(WERLD_SERVER_ADDRESS,
                            WERLD_SERVER_PORT,
                            &hints,
                            &results))) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "getaddrinfo: %s\n",
                     gai_strerror(status));
    exit(errno);
  }

  for (iterator = results; iterator; iterator = iterator->ai_next) {
    if ((werld_client.fd = socket(iterator->ai_family,
                                  iterator->ai_socktype,
                                  iterator->ai_protocol)) == -1) {
      continue;
    }

    if (connect(werld_client.fd,
        iterator->ai_addr,
        iterator->ai_addrlen) == -1) {
      close(werld_client.fd);
      continue;
    }

    break;
  }

  freeaddrinfo(results);

  if (iterator) {
    keepalive = 1;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(werld_client.fd,
                   SOL_SOCKET,
                   SO_KEEPALIVE,
                   &keepalive,
                   sizeof(keepalive)) == -1) {
      perror("setsockopt");
      exit(errno);
    }
    if (setsockopt(werld_client.fd,
                   SOL_SOCKET,
                   SO_RCVTIMEO,
                   &timeout,
                   sizeof(timeout)) == -1) {
      perror("setsockopt");
      exit(errno);
    }
    client_register(player);
    return(0);
  }

  return(-1);
}

int client_disconnect(struct player player) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_UNREGISTER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  void *offset =
    mempcpy(data, &WERLD_REQUEST_TYPE_UNREGISTER, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(offset, &player, sizeof(player));

  if ((bytes_written = net_write(werld_client.fd,
                                 data,
                                 WERLD_REQUEST_TYPE_UNREGISTER_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+disconnect write failed\n");
    exit(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_UNREGISTER_BUFSIZ,
                          "+client+disconnect bytes written: %zd ",
                          bytes_written);

  free(data);

  if (close(werld_client.fd)) {
    perror("close");
    exit(errno);
  }

  return(0);
}

int client_send_player(struct player player) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_PLAYER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  void *offset =
    mempcpy(data, &WERLD_REQUEST_TYPE_PLAYER, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(offset, &player, sizeof(struct player));

  if ((bytes_written = net_write(werld_client.fd,
                                 data,
                                 WERLD_REQUEST_TYPE_PLAYER_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+send_player write failed\n");
    exit(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_PLAYER_BUFSIZ,
                          "+client+send_player bytes written: %zd ",
                          bytes_written);
  free(data);

  return(0);
}

int client_request_players(void) {
  ssize_t bytes_written;

  if ((bytes_written = net_write(werld_client.fd,
                                 &WERLD_REQUEST_TYPE_PLAYERS,
                                 WERLD_REQUEST_TYPE_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+request_players write failed\n");
    exit(-1);
  }

  werld_client_log(WERLD_CLIENT_DEBUG,
                   "+client+request_players bytes written: %zd\n",
                   bytes_written);

  return(0);
}

int client_send_message(struct player player, const char *message) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message)))) {
    perror("malloc");
    return(-1);
  }

  void *offset =
    mempcpy(data, &WERLD_REQUEST_TYPE_MESSAGE, WERLD_REQUEST_TYPE_BUFSIZ);
  /* FIXME: Don't send entire player. */
  offset = mempcpy(offset, &player, sizeof(player));
  memcpy(offset, message, strlen(message));

  if ((bytes_written = net_write(werld_client.fd,
                                 data,
                                 WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+send_message write failed\n");
    exit(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message),
                          "+client+send_message bytes written: %zd ",
                          bytes_written);
  free(data);

  return(0);
}

int client_handle_response(void) {
  ssize_t bytes_read;
  uint32_t response_type;
  uint8_t *data;

  if ((bytes_read = net_read(werld_client.fd,
                             &response_type,
                             WERLD_RESPONSE_TYPE_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+handle_response read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          (uint8_t *) &response_type,
                          sizeof(uint32_t),
                          "+client+handle_response bytes read: %zd ",
                          bytes_read);

  if (response_type == WERLD_RESPONSE_TYPE_REGISTER) {
    struct player player;

    if (!(data = malloc(WERLD_RESPONSE_REGISTER_BUFSIZ))) {
      perror("malloc");
      exit(errno);
    }

    if ((bytes_read = net_read(werld_client.fd,
                               data,
                               WERLD_RESPONSE_REGISTER_BUFSIZ)) == -1) {
      werld_client_log(WERLD_CLIENT_ERROR, "+client+handle_response+register read failed\n");
      exit(-1);
    }

    if (bytes_read == 0) return(-1);

    werld_client_log_binary(WERLD_CLIENT_DEBUG,
                            data,
                            WERLD_RESPONSE_REGISTER_BUFSIZ,
                            "+client+handle_response+register bytes read: %zd ",
                            bytes_read);

    memcpy(&player, data, sizeof(struct player));
    free(data);
    memcpy(werld_client.player, &player, sizeof(struct player));
    ui_draw_player(*(werld_client.player));
    wrefresh(werld_client.window);
  } else if (response_type == WERLD_RESPONSE_TYPE_MESSAGE) {
    char *message;
    struct player player;
    uint32_t message_length;
    uint8_t *payload;

    if ((bytes_read = net_read(werld_client.fd,
                               &message_length,
                               sizeof(uint32_t))) == -1) {
      werld_client_log(WERLD_CLIENT_ERROR, "+client+handle_response+message read failed\n");
      exit(-1);
    }

    if (bytes_read == 0) return(-1);

    werld_client_log_binary(WERLD_CLIENT_DEBUG,
                            (uint8_t *) &message_length,
                            sizeof(uint32_t),
                            "+client+handle_response+message bytes read: %zd ",
                            bytes_read);

    if (!(message = malloc(message_length + 1))) {
      perror("malloc");
      exit(errno);
    }

    if (!(payload = malloc(WERLD_RESPONSE_MESSAGE_BUFSIZ(message_length)))) {
      perror("malloc");
      exit(errno);
    }

    if ((bytes_read = net_read(werld_client.fd,
                               payload,
                               WERLD_RESPONSE_MESSAGE_BUFSIZ(message_length))) == -1) {
      werld_client_log(WERLD_CLIENT_ERROR, "+client+handle_response+message read failed\n");
      exit(-1);
    }

    if (bytes_read == 0) return(-1);

    werld_client_log_binary(WERLD_CLIENT_DEBUG,
                            payload,
                            WERLD_RESPONSE_MESSAGE_BUFSIZ(message_length),
                            "+client+handle_response+message bytes read: %zd ",
                            bytes_read);

    memcpy(&player, payload, sizeof(struct player));
    memcpy(message, payload + sizeof(struct player), message_length);
    message[message_length] = '\0';

    message_handler_handle_incoming_message(&player, message);
    free(message);
    free(payload);
  } else if (response_type == WERLD_RESPONSE_TYPE_PLAYERS) {
    uint8_t *payload;
    uint32_t number_of_players;

    if ((bytes_read = net_read(werld_client.fd,
                               &number_of_players,
                               sizeof(uint32_t))) == -1) {
      werld_client_log(WERLD_CLIENT_ERROR, "+client+handle_response+players read failed\n");
      exit(-1);
    }

    if (bytes_read == 0) return(-1);

    werld_client_log_binary(WERLD_CLIENT_DEBUG,
                            (uint8_t *) &number_of_players,
                            sizeof(uint32_t),
                            "+client+handle_response+players bytes read: %zd ",
                            bytes_read);

    if (!(payload = malloc(WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players)))) {
      perror("malloc");
      exit(errno);
    }

    if ((bytes_read = net_read(werld_client.fd,
                               payload,
                               WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players))) == -1) {
      werld_client_log(WERLD_CLIENT_ERROR, "+client+handle_response+players read failed\n");
      exit(-1);
    }

    if (bytes_read == 0) return(-1);

    werld_client_log_binary(WERLD_CLIENT_DEBUG,
                            payload,
                            WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players),
                            "+client+handle_response+players bytes read: %zd ",
                            bytes_read);

    ui_erase_player_list(werld_client.player_list);
    player_list_update(&(werld_client.player_list),
                       (void *) payload,
                       number_of_players);
    ui_draw_player_list(werld_client.player_list);
    wrefresh(werld_client.window);
    free(payload);
  }

  return(0);
}
