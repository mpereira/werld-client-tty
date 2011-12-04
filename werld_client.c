#include <curses.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "client.h"
#include "main_window.h"
#include "message_bar_window.h"
#include "message_handler.h"
#include "status_bar_window.h"
#include "werld_client.h"

static const size_t WERLD_LOG_MESSAGE_BUFSIZ = 1024;
static const char *WERLD_CLIENT_LOG_LEVEL[] = { "debug", "error", "info" };

struct werld_client werld_client;

void werld_client_log(int level, const char *fmt, ...) {
  FILE *log_file;
  char message[WERLD_LOG_MESSAGE_BUFSIZ];
  char time_str[16];
  time_t now = time(NULL);
  va_list ap;

  if (level < werld_client.log_level) return;

  va_start(ap, fmt);
  vsnprintf(message, sizeof(message), fmt, ap);
  va_end(ap);

  log_file = (werld_client.log_file == NULL) ? stderr :
                                               fopen(werld_client.log_file, "a");

  if (!log_file) return;

  strftime(time_str, sizeof(time_str), "%d %b %X", localtime(&now));
  fprintf(log_file, "%s [%s] %s", time_str, WERLD_CLIENT_LOG_LEVEL[level], message);
  fflush(log_file);

  if (werld_client.log_file) fclose(log_file);
}

void werld_client_log_binary(int level,
                             const uint8_t *binary,
                             size_t binary_size,
                             char *fmt, ...) {
  FILE *log_file;
  char message[WERLD_LOG_MESSAGE_BUFSIZ];
  va_list ap;

  if (level < werld_client.log_level) return;

  if (werld_client.log_file == NULL) {
    log_file = stderr;
  } else {
    if (!(log_file = fopen(werld_client.log_file, "a"))) {
      perror("fopen");
      exit(errno);
    }
  }

  if (!log_file) return;

  va_start(ap, fmt);
  vsnprintf(message, sizeof(message), fmt, ap);
  va_end(ap);

  werld_client_log(level, message);

  fprintf(log_file, "<<");
  for (unsigned int i = 0; i < binary_size - 1; i++) {
    fprintf(log_file, "%hd,", binary[i]);
  }
  fprintf(log_file, "%hd", binary[binary_size]);
  fprintf(log_file, ">>\n");
  fflush(log_file);

  if (werld_client.log_file) fclose(log_file);
}

void werld_client_init(struct werld_client *werld_client) {
  werld_client->message_bar_window = NULL;
  werld_client->status_bar_window = NULL;
  werld_client->main_window = NULL;

  werld_client->log_file = NULL;

  werld_client->fd = -1;
  for (int i = 0; i < 2; i++) werld_client->message_handler_fds[i] = -1;

  werld_client->world_map = NULL;
  werld_client->player = NULL;
  werld_client->player_list = NULL;

  initscr();
  cbreak();
  curs_set(false);
}

void werld_client_kill(struct werld_client *werld_client) {
  if (werld_client->message_bar_window) message_bar_window_del(werld_client->message_bar_window);
  if (werld_client->status_bar_window) status_bar_window_del(werld_client->status_bar_window);
  if (werld_client->main_window) main_window_del(werld_client->main_window);

  if (werld_client->log_file) free(werld_client->log_file);

  if (werld_client->fd != -1) client_request_disconnect(werld_client->player);

  for (int i = 0; i < 2; i++)
    if (werld_client->message_handler_fds[i] != -1)
      message_handler_close(werld_client->message_handler_fds[i]);

  if (werld_client->world_map) map_free(&(werld_client->world_map));
  if (werld_client->player_list) player_list_free(&(werld_client->player_list));

  endwin();
}
