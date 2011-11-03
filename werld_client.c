#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "werld_client.h"

char *werld_client_log_level[] = { "debug", "error", "info" };

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
  fprintf(log_file, "%s [%s] %s", time_str, werld_client_log_level[level], message);
  fflush(log_file);
}

void werld_client_log_binary(int level, const char binary[], size_t binary_size, char *fmt, ...) {
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

  for (unsigned int i = 0; i < binary_size; i++) {
    fprintf(log_file, "%x", binary[i]);
  }
  fprintf(log_file, "\n");
}
