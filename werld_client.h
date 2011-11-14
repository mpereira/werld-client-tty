#ifndef WERLD_WERLD_CLIENT_H
#define WERLD_WERLD_CLIENT_H

#include <curses.h>
#include <stdarg.h>

#include "player.h"
#include "player_list.h"

static const size_t WERLD_LOG_MESSAGE_BUFSIZ = 1024;
static const size_t WERLD_BINARY_STRING_BUFSIZ = 896;

enum { WERLD_CLIENT_DEBUG, WERLD_CLIENT_ERROR, WERLD_CLIENT_INFO };

#define werld_client_log_binary(level, binary, fmt, ...) \
  _werld_client_log_binary(level, binary, sizeof(binary), fmt, ## __VA_ARGS__)

struct werld_client {
  WINDOW *message_bar;
  char *log_file;
  double player_messages_lifetime;
  int fd;
  int log_level;
  struct player *player;
  struct player_list *player_list;
};

extern struct werld_client werld_client;

void werld_client_log(int, const char *, ...);
void _werld_client_log_binary(int, const char[], size_t, char *, ...);

#endif
