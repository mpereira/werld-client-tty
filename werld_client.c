#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "player.h"
#include "player_list.h"
#include "werld_client.h"

int werld_client_connect(void) {
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

  if ((bytes_read = read(fd, &response, SEND_PLAYER_BUFFER_SIZE)) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
  fprintf(stderr, "[send_player] bytes read: %zd\n", bytes_read);

  return(atoi(response));
}

struct player_list *werld_client_get_players(void) {
  struct player_list *player_list;
  char response[GET_PLAYERS_BUFFER_SIZE];
  ssize_t bytes_written, bytes_read;
  int number_of_players;

  if ((bytes_written = write(fd, GET_PLAYERS, strlen(GET_PLAYERS))) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
  fprintf(stderr, "[get_players] bytes written: %zd\n", bytes_written);

  if ((bytes_read = read(fd, response, GET_PLAYERS_BUFFER_SIZE)) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
  fprintf(stderr, "[get_players] bytes read: %zd ", bytes_read);
  for (int i = 0; i < bytes_read; i++) {
    fprintf(stderr, "%x", response[i]);
  }
  fprintf(stderr, "\n");

  memcpy(&number_of_players, response, 4);
  fprintf(stderr, "[get_players] number of players: %d\n", number_of_players);

  struct player players[number_of_players];

  memcpy(players,
         response + 4,
         number_of_players * sizeof(struct player));

  player_list_init(&player_list);
  player_list_fill(&player_list, players, number_of_players);

  return(player_list);
}
