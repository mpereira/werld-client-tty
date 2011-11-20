#ifndef WERLD_TTY_H
#define WERLD_TTY_H

extern const char *WERLD_SMALL_TERM_MSG;

bool tty_term_size_ok(void);
void tty_handle_resize(void);

#endif
