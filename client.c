#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include "client.h"
#include "player.h"
#include "player_list.h"
#include "ui.h"

int fd;

static void client_register(struct player player) {
  ssize_t bytes_written;
  char payload[REQUEST_REGISTER_BUFSIZ];

  memcpy(payload, REQUEST_REGISTER, strlen(REQUEST_REGISTER));
  memcpy(payload + strlen(REQUEST_REGISTER), &player, sizeof(struct player));

  if ((bytes_written = write(fd, payload, sizeof(payload))) < 0) {
    perror("write");
    exit(errno);
  }
  fprintf(stderr, "[register] bytes written: %zd ", bytes_written);
  for (int i = 0; i < bytes_written; i++) {
    fprintf(stderr, "%x", ((char *) payload)[i]);
  }
  fprintf(stderr, "\n");
}

int client_connect(struct player player) {
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
    client_register(player);
    return(0);
  }

  return(-1);
}

void client_disconnect(void) {
  ssize_t bytes_written;
  char payload[REQUEST_UNREGISTER_BUFSIZ];

  memcpy(payload, REQUEST_UNREGISTER, strlen(REQUEST_UNREGISTER));
  memcpy(payload + strlen(REQUEST_UNREGISTER), &player, sizeof(player));

  if ((bytes_written = write(fd, &payload, sizeof(payload))) < 0) {
    perror("write");
    exit(errno);
  }
  fprintf(stderr, "[disconnect] bytes written: %zd ", bytes_written);
  for (int i = 0; i < bytes_written; i++) {
    fprintf(stderr, "%x", ((char *) &payload)[i]);
  }
  fprintf(stderr, "\n");

  if (close(fd)) {
    perror("close");
    exit(errno);
  }
}

int client_send_player(struct player player) {
  ssize_t bytes_written;
  char payload[REQUEST_PLAYER_BUFSIZ];

  memcpy(payload, REQUEST_PLAYER, strlen(REQUEST_PLAYER));
  memcpy(payload + strlen(REQUEST_PLAYER), &player, sizeof(player));

  if ((bytes_written = write(fd, &payload, sizeof(payload))) < 0) {
    perror("write");
    exit(errno);
  }
  fprintf(stderr, "[send_player] bytes written: %zd ", bytes_written);
  for (int i = 0; i < bytes_written; i++) {
    fprintf(stderr, "%x", ((char *) &payload)[i]);
  }
  fprintf(stderr, "\n");

  return(1);
}

void client_request_players(void) {
  ssize_t bytes_written;

  if ((bytes_written = write(fd,
                             REQUEST_PLAYERS,
                             strlen(REQUEST_PLAYERS))) < 0) {
    perror("write");
    exit(errno);
  }
  fprintf(stderr, "[request_players] bytes written: %zd\n", bytes_written);
}

int client_handle_response(void) {
  ssize_t bytes_read;
  uint32_t event;

  if ((bytes_read = read(fd, &event, sizeof(EVENT_HEADER_BUFSIZ))) < 0) {
    perror("read");
    exit(errno);
  }

  if (bytes_read == 0) {
    return(-1);
  }

  if (event == EVENT_MESSAGE) {
    uint32_t message_length;

    if ((bytes_read = read(fd, &message_length, sizeof(uint32_t))) < 0) {
      perror("read");
      exit(errno);
    }

    fprintf(stderr, "[event_message] bytes read: %zd ", bytes_read);
    for (int i = 0; i < bytes_read; i++) {
      fprintf(stderr, "%x", ((char *) &message_length)[i]);
    }
    fprintf(stderr, "\n");

    if (bytes_read == 0) {
      return(-1);
    }

    char message[message_length + 1];
    char event_message_payload[sizeof(message) + sizeof(struct player)];

    if ((bytes_read = read(fd,
                           event_message_payload,
                           sizeof(event_message_payload))) < 0) {
      perror("read");
      exit(errno);
    }

    if (bytes_read == 0) {
      return(-1);
    }

    fprintf(stderr, "[event_message] bytes read: %zd ", bytes_read);
    for (int i = 0; i < bytes_read; i++) {
      fprintf(stderr, "%x", event_message_payload[i]);
    }
    fprintf(stderr, "\n");

    struct player player;

    memcpy(&player, event_message_payload, sizeof(struct player));
    memcpy(message,
           event_message_payload + sizeof(struct player),
           sizeof(message) - 1);
    message[sizeof(message)] = '\0';

    ui_draw_player_list(player_list);
    ui_draw_player_with_message(player, message);
  } else if (event == EVENT_PLAYERS) {
    uint32_t number_of_players;

    if ((bytes_read = read(fd, &number_of_players, sizeof(uint32_t))) < 0) {
      perror("read");
      exit(errno);
    }

    fprintf(stderr, "[event_players] bytes read: %zd ", bytes_read);
    for (int i = 0; i < bytes_read; i++) {
      fprintf(stderr, "%x", ((char *) &number_of_players)[i]);
    }
    fprintf(stderr, "\n");

    if (bytes_read == 0) {
      return(-1);
    }

    char event_players_payload[number_of_players * sizeof(struct player)];

    if ((bytes_read = read(fd,
                           event_players_payload,
                           sizeof(event_players_payload))) < 0) {
      perror("read");
      exit(errno);
    }

    if (bytes_read == 0) {
      return(-1);
    }

    fprintf(stderr, "[event_players] bytes read: %zd ", bytes_read);
    for (int i = 0; i < bytes_read; i++) {
      fprintf(stderr, "%x", event_players_payload[i]);
    }
    fprintf(stderr, "\n");

    player_list_update(&player_list, (void *) event_players_payload, number_of_players);

    ui_draw_player_list(player_list);
    ui_draw_player(player);
  }

  return(0);
}

void client_send_message(struct player player, const char *message) {
  char payload[REQUEST_MESSAGE_BUFSIZ];
  ssize_t bytes_written;

  void *offset = mempcpy(payload, REQUEST_MESSAGE, strlen(REQUEST_MESSAGE));
  /* FIXME: Don't send entire player. */
  offset = mempcpy(offset, &player, sizeof(player));
  memcpy(offset, message, strlen(message));

  ssize_t payload_size = strlen(REQUEST_MESSAGE) +
                           sizeof(player) +
                           strlen(message);

  if ((bytes_written = write(fd, &payload, payload_size)) < 0) {
    perror("write");
    exit(errno);
  }
  fprintf(stderr, "[send_message] bytes written: %zd ", bytes_written);
  for (int i = 0; i < bytes_written; i++) {
    fprintf(stderr, "%x", ((char *) &payload)[i]);
  }
  fprintf(stderr, "\n");
}
