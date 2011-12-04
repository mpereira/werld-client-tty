#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "main_window.h"
#include "maps.h"
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

const uint8_t WERLD_RESPONSE_TYPE_ERROR    = -1;
const uint8_t WERLD_RESPONSE_TYPE_REGISTER = 0;
const uint8_t WERLD_RESPONSE_TYPE_PLAYERS  = 1;
const uint8_t WERLD_RESPONSE_TYPE_MESSAGE  = 2;
const uint8_t WERLD_RESPONSE_TYPE_MAP      = 3;

static const size_t WERLD_RESPONSE_TYPE_BUFSIZ = sizeof(uint8_t);

#define WERLD_RESPONSE_REGISTER_BUFSIZ sizeof(struct player)
#define WERLD_RESPONSE_MESSAGE_BUFSIZ(message_length) (sizeof(struct player) + \
                                                       message_length)
#define WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players) (number_of_players * \
                                                          sizeof(struct player))

static void client_request_register(struct player player) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_REGISTER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  memcpy(data, &WERLD_REQUEST_TYPE_REGISTER, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(data + WERLD_REQUEST_TYPE_BUFSIZ, &player, sizeof(struct player));

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

int client_request_connect(struct player player) {
  int status;
  struct addrinfo hints;
  struct addrinfo *results;

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

  struct addrinfo *iterator;

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
    client_request_register(player);
    return(0);
  }

  return(-1);
}

int client_request_disconnect(struct player *player) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_UNREGISTER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  memcpy(data, &WERLD_REQUEST_TYPE_UNREGISTER, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(data + WERLD_REQUEST_TYPE_BUFSIZ, player, sizeof(struct player));

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

int client_request_player(struct player player) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_PLAYER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  memcpy(data, &WERLD_REQUEST_TYPE_PLAYER, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(data + WERLD_REQUEST_TYPE_BUFSIZ, &player, sizeof(struct player));

  if ((bytes_written = net_write(werld_client.fd,
                                 data,
                                 WERLD_REQUEST_TYPE_PLAYER_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+request+player write failed\n");
    exit(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_PLAYER_BUFSIZ,
                          "+client+request+player bytes written: %zd ",
                          bytes_written);
  free(data);

  return(0);
}

int client_request_players(void) {
  ssize_t bytes_written;

  if ((bytes_written = net_write(werld_client.fd,
                                 &WERLD_REQUEST_TYPE_PLAYERS,
                                 WERLD_REQUEST_TYPE_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+request_players write failed\n");
    exit(-1);
  }

  werld_client_log(WERLD_CLIENT_DEBUG,
                   "+client+request_players bytes written: %zd\n",
                   bytes_written);

  return(0);
}

int client_request_message(struct player player, const char *message) {
  ssize_t bytes_written;
  uint8_t *data;

  if (!(data = malloc(WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message)))) {
    perror("malloc");
    return(-1);
  }

  char *offset =
    memcpy(data, &WERLD_REQUEST_TYPE_MESSAGE, WERLD_REQUEST_TYPE_BUFSIZ);
  /* FIXME: Don't send entire player. */
  offset = memcpy(offset + WERLD_REQUEST_TYPE_BUFSIZ, &player, sizeof(player));
  memcpy(offset + sizeof(player), message, strlen(message));

  if ((bytes_written =
         net_write(werld_client.fd,
                   data,
                   WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+request+message write failed\n");
    exit(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message),
                          "+client+request+message bytes written: %zd ",
                          bytes_written);
  free(data);

  return(0);
}

int client_request_map(uint8_t map) {
  ssize_t bytes_written;
  uint8_t *data;

  if (map != WERLD_MAPS_WORLD) return(-1);

  if (!(data = malloc(WERLD_REQUEST_TYPE_MAP_PACKET_SIZE))) {
    perror("malloc");
    return(-1);
  }

  memcpy(data, &WERLD_REQUEST_TYPE_MAP, WERLD_REQUEST_TYPE_BUFSIZ);
  memcpy(data + WERLD_REQUEST_TYPE_BUFSIZ,
         &WERLD_MAPS_WORLD,
         WERLD_MAPS_HEADER_FIELD_SIZE);

  if ((bytes_written = net_write(werld_client.fd,
                                 data,
                                 WERLD_REQUEST_TYPE_MAP_PACKET_SIZE)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR, "+client+request+map write failed\n");
    return(-1);
  }

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          data,
                          WERLD_REQUEST_TYPE_MAP_PACKET_SIZE,
                          "+client+request+map bytes written: %zd ",
                          bytes_written);
  free(data);

  return(0);
}

static int client_handle_response_register(void) {
  ssize_t bytes_read;
  struct player player;
  uint8_t *data;

  if (!(data = malloc(WERLD_RESPONSE_REGISTER_BUFSIZ))) {
    perror("malloc");
    exit(errno);
  }

  if ((bytes_read = net_read(werld_client.fd,
                             data,
                             WERLD_RESPONSE_REGISTER_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+register read failed\n");
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
  main_window_refresh(werld_client.main_window);

  return(0);
}

static int client_handle_response_message(void) {
  char *message;
  ssize_t bytes_read;
  struct player player;
  uint32_t message_length;
  uint8_t *payload;

  if ((bytes_read = net_read(werld_client.fd,
                             &message_length,
                             sizeof(uint32_t))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+message read failed\n");
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

  if ((bytes_read =
         net_read(werld_client.fd,
                  payload,
                  WERLD_RESPONSE_MESSAGE_BUFSIZ(message_length))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+message read failed\n");
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

  return(0);
}

static int client_handle_response_players(void) {
  ssize_t bytes_read;
  uint32_t number_of_players;
  uint8_t *payload;

  if ((bytes_read = net_read(werld_client.fd,
                             &number_of_players,
                             sizeof(uint32_t))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+players read failed\n");
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

  if ((bytes_read =
         net_read(werld_client.fd,
                  payload,
                  WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+players read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          payload,
                          WERLD_RESPONSE_PLAYERS_BUFSIZ(number_of_players),
                          "+client+handle_response+players bytes read: %zd ",
                          bytes_read);

  ui_erase_player_list(werld_client.player_list);
  ui_draw_map(werld_client.world_map);
  player_list_update(&(werld_client.player_list),
                     (void *) payload,
                     number_of_players);
  ui_draw_player_list(werld_client.player_list);
  main_window_refresh(werld_client.main_window);
  free(payload);

  return(0);
}

static int client_handle_response_map(void) {
  size_t tiles_size;
  ssize_t bytes_read;
  struct map **map;
  uint32_t dimensions[2];
  uint8_t *payload;
  uint8_t _map;

  if ((bytes_read = net_read(werld_client.fd,
                             &_map,
                             sizeof(uint8_t))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+map read failed\n");
    return(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          &_map,
                          sizeof(uint8_t),
                          "+client+handle_response+map bytes read: %zd ",
                          bytes_read);

  if (_map == WERLD_MAPS_WORLD) {
    map = &(werld_client.world_map);
  } else {
    return(-1);
  }

  if ((bytes_read = net_read(werld_client.fd,
                             &dimensions,
                             2 * sizeof(uint32_t))) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+map read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          (uint8_t *) &dimensions,
                          2 * sizeof(uint32_t),
                          "+client+handle_response+map bytes read: %zd ",
                          bytes_read);

  tiles_size = WERLD_MAP_TILES_SIZE(dimensions[0], dimensions[1]);

  if (!(payload = malloc(tiles_size))) {
    perror("malloc");
    exit(errno);
  }

  if ((bytes_read = net_read(werld_client.fd, payload, tiles_size)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response+map read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          payload,
                          tiles_size,
                          "+client+handle_response+map bytes read: %zd ",
                          bytes_read);

  map_malloc(map, dimensions[0], dimensions[1]);
  (*map)->width = dimensions[0];
  (*map)->height = dimensions[1];
  for (int i = 0; (uint32_t) i < (*map)->width; i++) {
    memcpy((*map)->tiles[i],
           payload + (*map)->height * sizeof(**((*map)->tiles)) * i,
           (*map)->height * sizeof(**((*map)->tiles)));
  }

  return(0);
}

int client_handle_response(void) {
  ssize_t bytes_read;
  uint8_t response_type;

  if ((bytes_read = net_read(werld_client.fd,
                             &response_type,
                             WERLD_RESPONSE_TYPE_BUFSIZ)) == -1) {
    werld_client_log(WERLD_CLIENT_ERROR,
                     "+client+handle_response read failed\n");
    exit(-1);
  }

  if (bytes_read == 0) return(-1);

  werld_client_log_binary(WERLD_CLIENT_DEBUG,
                          (uint8_t *) &response_type,
                          sizeof(uint8_t),
                          "+client+handle_response bytes read: %zd ",
                          bytes_read);

  if (response_type == WERLD_RESPONSE_TYPE_REGISTER) {
    return(client_handle_response_register());
  } else if (response_type == WERLD_RESPONSE_TYPE_MESSAGE) {
    return(client_handle_response_message());
  } else if (response_type == WERLD_RESPONSE_TYPE_PLAYERS) {
    return(client_handle_response_players());
  } else if (response_type == WERLD_RESPONSE_TYPE_MAP) {
    return(client_handle_response_map());
  }

  return(0);
}
