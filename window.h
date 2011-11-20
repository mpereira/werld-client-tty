#ifndef WERLD_WINDOW_H
#define WERLD_WINDOW_H

#include <curses.h>

extern const int WERLD_ACCOUNT_MAX_SIZE;
extern const int WERLD_PASSWORD_MAX_SIZE;

void window_init(WINDOW **);
void window_get_credentials(WINDOW *, char *, char *);

#endif
