#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include "player.h"
#include "player_list.h"
#include "werld_client.h"

static void werld_client_register(const struct player player) {
  ssize_t bytes_written;
  char payload[REGISTER_BUFFER_SIZE];

  memcpy(payload, REGISTER, strlen(REGISTER));
  memcpy(payload + strlen(REGISTER), &player, sizeof(player));

  if ((bytes_written = write(fd, &payload, sizeof(payload))) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
  fprintf(stderr, "[register] bytes written: %zd ", bytes_written);
  for (int i = 0; i < bytes_written; i++) {
    fprintf(stderr, "%x", ((char *) &payload)[i]);
  }
  fprintf(stderr, "\n");
}

int werld_client_connect(const struct player player) {
  int status;
  struct addrinfo hints;
  struct addrinfo *results;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(SERVER_ADDRESS, SERVER_PORT, &hints, &results))) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(errno);
  }

  struct addrinfo *iterator;

  for (iterator = results; iterator; iterator = iterator->ai_next) {
    if ((fd = socket(iterator->ai_family,
                     iterator->ai_socktype,
                     iterator->ai_protocol)) == -1) {
      continue;
    }

    if (connect(fd, iterator->ai_addr, iterator->ai_addrlen) == -1) {
      close(fd);
      continue;
    }

    break;
  }

  freeaddrinfo(results);

  if (iterator) {
    werld_client_register(player);
    return(0);
  }

  return(-1);
}

void werld_client_disconnect(void) {
  if (close(fd)) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
}

int werld_client_send_player(struct player player) {
  char response[SEND_PLAYER_BUFFER_SIZE];
  ssize_t bytes_read, bytes_written;

  if ((bytes_written = write(fd, &player, sizeof(player))) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
  fprintf(stderr, "[send_player] bytes written: %zd ", bytes_written);
  for (int i = 0; i < bytes_written; i++) {
    fprintf(stderr, "%x", ((char *) &player)[i]);
  }
  fprintf(stderr, "\n");

  return(1);
}

struct player_list *werld_client_get_players(void) {
  ssize_t bytes_written;

  if ((bytes_written = write(fd, GET_PLAYERS, strlen(GET_PLAYERS))) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
  fprintf(stderr, "[get_players] bytes written: %zd\n", bytes_written);

  return(werld_client_handle_server_response());
}

struct player_list *werld_client_handle_server_response() {
  struct player_list *player_list;
  char response[GET_PLAYERS_BUFFER_SIZE];
  ssize_t bytes_read;
  int number_of_players;

  if ((bytes_read = read(fd, response, GET_PLAYERS_BUFFER_SIZE)) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  fprintf(stderr, "[handle_server_response] bytes read: %zd ", bytes_read);
  for (int i = 0; i < bytes_read; i++) {
    fprintf(stderr, "%x", response[i]);
  }
  fprintf(stderr, "\n");

  memcpy(&number_of_players, response, 4);
  fprintf(stderr, "[handle_server_response] number of players: %d\n", number_of_players);

  struct player players[number_of_players];

  memcpy(players,
         response + 4,
         number_of_players * sizeof(struct player));

  player_list_init(&player_list);
  player_list_fill(&player_list, players, number_of_players);

  return(player_list);
}
