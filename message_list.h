#ifndef WERLD_MESSAGE_LIST_H
#define WERLD_MESSAGE_LIST_H

struct message_list {
  char *message;
  struct message_list *next;
};

void message_list_malloc(struct message_list **);
int message_list_add(struct message_list **, const char *message);
int message_list_remove(struct message_list **, const char *message);
void message_list_free(struct message_list *);

#endif
