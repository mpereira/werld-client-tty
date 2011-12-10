#ifndef WERLD_MAIN_WINDOW_H
#define WERLD_MAIN_WINDOW_H

#include <curses.h>

extern const int WERLD_ACCOUNT_MAX_SIZE;
extern const int WERLD_PASSWORD_MAX_SIZE;

void main_window_new(WINDOW **);
void main_window_init(WINDOW *);
void main_window_draw_login_screen(WINDOW *);
void main_window_get_credentials(WINDOW *, char *, char *);
void main_window_del(WINDOW *);
void main_window_refresh(WINDOW *);

#endif
