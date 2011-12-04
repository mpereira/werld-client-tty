#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static ssize_t
net_exchange(int fd,
             void *buffer,
             size_t nbyte,
             ssize_t (*function)(int fd, void *buffer, size_t nbyte)) {
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
  return(net_exchange(fd,
                      (void *) buffer,
                      nbyte,
                      (ssize_t (*)(int, void *, size_t)) write));
}

ssize_t net_read(int fd, void *buffer, size_t nbyte) {
  return(net_exchange(fd, buffer, nbyte, read));
}
