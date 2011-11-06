#ifndef WERLD_MESSAGE_LIST_H
#define WERLD_MESSAGE_LIST_H

#include <time.h>

struct message_list {
  char *message;
  time_t created_at;
  struct message_list *next;
};

void message_list_malloc(struct message_list **);
int message_list_add(struct message_list **, const char *, time_t);
int message_list_remove(struct message_list **, const char *);
void message_list_free(struct message_list *);

#endif
