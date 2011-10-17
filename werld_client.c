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
#include "werld_client.h"

#define BUFFER_SIZE 100

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
  char response[BUFFER_SIZE];

  write(fd, &player, sizeof(player));
  read(fd, &response, BUFFER_SIZE - 1);

  return(atoi(response));
}
