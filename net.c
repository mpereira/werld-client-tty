#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

static ssize_t
net_socket_exchange(int socket,
                    void *buffer,
                    size_t nbyte,
                    int flags,
                    ssize_t (*function)(int, void *, size_t, int)) {
  ssize_t bytes_exchanged;
  ssize_t total_bytes_exchanged;

  total_bytes_exchanged = 0;

  while (total_bytes_exchanged < (ssize_t) nbyte) {
    if ((bytes_exchanged = (*function)(socket, buffer, nbyte, flags)) < 0) {
      perror((function == recv) ? "recv" : "send");
      exit(errno);
    }
    total_bytes_exchanged += bytes_exchanged;
  }

  return((total_bytes_exchanged == (ssize_t) nbyte) ? total_bytes_exchanged : -1);
}

ssize_t net_send(int socket, const void *buffer, size_t nbyte, int flags) {
  return(net_socket_exchange(socket,
                             (void *) buffer,
                             nbyte,
                             flags,
                             (ssize_t (*)(int, void *, size_t, int)) send));
}

ssize_t net_recv(int socket, void *buffer, size_t nbyte, int flags) {
  return(net_socket_exchange(socket, buffer, nbyte, flags, recv));
}

static ssize_t net_fd_exchange(int fd,
                               void *buffer,
                               size_t nbyte,
                               ssize_t (*function)(int, void *, size_t)) {
  ssize_t bytes_exchanged;
  ssize_t total_bytes_exchanged;

  total_bytes_exchanged = 0;

  while (total_bytes_exchanged < (ssize_t) nbyte) {
    if ((bytes_exchanged = (*function)(fd, buffer, nbyte)) < 0) {
      perror((function == read) ? "read" : "write");
      exit(errno);
    }
    total_bytes_exchanged += bytes_exchanged;
  }

  return((total_bytes_exchanged == (ssize_t) nbyte) ? total_bytes_exchanged : -1);
}

ssize_t net_write(int fd, const void *buffer, size_t nbyte) {
  return(net_fd_exchange(fd,
                      (void *) buffer,
                      nbyte,
                      (ssize_t (*)(int, void *, size_t)) write));
}

ssize_t net_read(int fd, void *buffer, size_t nbyte) {
  return(net_fd_exchange(fd, buffer, nbyte, read));
}
