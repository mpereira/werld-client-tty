#ifndef WERLD_NET_H
#define WERLD_NET_H

#include <stddef.h>

ssize_t net_write(int, const void *, size_t);
ssize_t net_read(int, void *, size_t);

#endif
