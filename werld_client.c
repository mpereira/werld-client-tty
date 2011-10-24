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

void werld_client_connect(void) {
  struct sockaddr_in socket_address;

  memset(&socket_address, 0, sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons(SERVER_PORT);

  if (!inet_aton(SERVER_ADDRESS, &(socket_address.sin_addr))) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  if ((fd = socket(socket_address.sin_family, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  if (connect(fd, &socket_address, sizeof(socket_address))) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }
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

  if ((bytes_read = read(fd, &response, SEND_PLAYER_BUFFER_SIZE)) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  fprintf(stderr,
          "[send_player] bytes written: %zd, bytes read: %zd\n",
          bytes_written,
          bytes_read);

  return(atoi(response));
}

struct player_list *werld_client_get_players(void) {
  struct player_list *player_list, *head;
  char *response;
  ssize_t bytes_written, bytes_read;
  int number_of_players;

  if (!(response = malloc(GET_PLAYERS_BUFFER_SIZE))) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  if ((bytes_written = write(fd, GET_PLAYERS, strlen(GET_PLAYERS))) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  if ((bytes_read = read(fd, response, GET_PLAYERS_BUFFER_SIZE)) < 0) {
    fprintf(stderr, strerror(errno));
    exit(errno);
  }

  fprintf(stderr, "[get_players] bytes written: %zd, bytes read: %zd\n", bytes_written, bytes_read);

  memcpy(&number_of_players, response, 4);
  fprintf(stderr, "[get_players] number of players: %d\n", number_of_players);

  if (number_of_players > 0) {
    if (!(player_list = malloc(sizeof(struct player_list)))) {
      fprintf(stderr, strerror(errno));
      exit(errno);
    }
  } else {
    player_list = NULL;
  }

  head = player_list;

  for (int i = 0; i < number_of_players; i++) {
    if (!(player_list->player = malloc(sizeof(struct player)))) {
      fprintf(stderr, strerror(errno));
      exit(errno);
    }

    memcpy(player_list->player,
           (response + 4) + (i * sizeof(struct player)),
           sizeof(struct player));
    fprintf(stderr,
            "[get_players] #player{id=%d, name=\"%s\", y=%d, x=%d}\n",
            player_list->player->id,
            player_list->player->name,
            player_list->player->y,
            player_list->player->x);

    if (i == (number_of_players - 1)) {
      player_list->next = NULL;
    } else {
      if (!(player_list->next = malloc(sizeof(struct player)))) {
        fprintf(stderr, strerror(errno));
        exit(errno);
      }
      player_list = player_list->next;
    }
  }

  return(head);
}
