#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

static ssize_t
net_exchange(int fd,
             void *buffer,
             size_t nbyte,
             int flags,
             ssize_t (*function)(int fd, void *buffer, size_t nbyte, int flags)) {
  ssize_t bytes_exchanged;
  ssize_t total_bytes_exchanged;

  total_bytes_exchanged = 0;

  while (total_bytes_exchanged < (ssize_t) nbyte) {
    if ((bytes_exchanged = (*function)(fd, buffer, nbyte, flags)) < 0) {
      perror((function == recv) ? "recv" : "send");
      exit(errno);
    }
    total_bytes_exchanged += bytes_exchanged;
  }

  return((total_bytes_exchanged == (ssize_t) nbyte) ? total_bytes_exchanged : -1);
}

ssize_t net_write(int fd, const void *buffer, size_t nbyte) {
  return(net_exchange(fd,
                      (void *) buffer,
                      nbyte,
                      0,
                      (ssize_t (*)(int, void *, size_t, int)) send));
}

ssize_t net_read(int fd, void *buffer, size_t nbyte) {
  int flags;
  flags = MSG_WAITALL;
  return(net_exchange(fd, buffer, nbyte, flags, recv));
}
