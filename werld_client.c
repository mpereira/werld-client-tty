#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

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

  log_file =
    (werld_client.log_file == NULL) ? stderr : fopen(werld_client.log_file, "a");

  if (!log_file) return;

  strftime(time_str, sizeof(time_str), "%d %b %X", localtime(&now));
  fprintf(log_file, "%s [%s] %s", time_str, WERLD_CLIENT_LOG_LEVEL[level], message);
  fflush(log_file);
}

void werld_client_log_binary(int level,
                             const uint8_t *binary,
                             size_t binary_size,
                             char *fmt, ...) {
  FILE *log_file;
  char message[WERLD_LOG_MESSAGE_BUFSIZ];
  va_list ap;

  if (level < werld_client.log_level) return;

  log_file =
    (werld_client.log_file == NULL) ? stderr : fopen(werld_client.log_file, "a");

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
}
