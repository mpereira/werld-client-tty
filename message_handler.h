#ifndef WERLD_MESSAGE_HANDLER_H
#define WERLD_MESSAGE_HANDLER_H

#include "player.h"

int message_handler_handle_player_message(void);
void message_handler_handle_incoming_message(const struct player *, const char *);
void message_handler_sweep_messages(void);

#endif
