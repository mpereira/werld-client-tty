#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "message_list.h"

static size_t message_list_malloc_message(char **storage, const char *message) {
  size_t buffer_size = WERLD_PLAYER_MESSAGE_BUFSIZ;
  size_t message_size = strlen(message) + 1;
  size_t size;

  size = (message_size > buffer_size) ? buffer_size : message_size;

  if (!(*storage = malloc(size))) {
    perror("malloc");
    exit(errno);
  }

  return(size);
}

void message_list_malloc(struct message_list **message_list) {
  if (!(*message_list = malloc(sizeof(struct message_list)))) {
    perror("malloc");
    exit(errno);
  }
}

int message_list_add(struct message_list **message_list, const char *message) {
  size_t size;

  if (!*message_list) {
    message_list_malloc(message_list);
    (*message_list)->message = NULL;
    (*message_list)->next = NULL;
    size = message_list_malloc_message(&((*message_list)->message), message);
    strncpy((*message_list)->message, message, size);
    return(1);
  }

  struct message_list *iterator = *message_list;
  for (; iterator->next; iterator = iterator->next)
    ;

  message_list_malloc(&(iterator->next));
  iterator->next->message = NULL;
  iterator->next->next = NULL;
  size = message_list_malloc_message(&(iterator->next->message), message);
  strncpy(iterator->next->message, message, size);

  return(1);
}

int message_list_remove(struct message_list **message_list, const char *message) {
  struct message_list *iterator = *message_list;
  struct message_list *tmp;

  if (!*message_list) return(0);

  if (iterator->next) {
    for (; iterator; iterator = iterator->next) {
      if (iterator->next && !strcmp(iterator->next->message, message)) {
        tmp = iterator->next;
        iterator->next = iterator->next->next;
        free(tmp->message);
        free(tmp);
        return(1);
      }
    }
  } else {
    if (!strcmp((*message_list)->message, message)) {
      message_list_free(*message_list);
      *message_list = NULL;
      return(1);
    }
  }

  return(0);
}

void message_list_free(struct message_list *message_list) {
  struct message_list *tmp;

  while (message_list) {
    tmp = message_list;
    free(message_list->message);
    message_list = message_list->next;
    free(tmp);
  }
}
