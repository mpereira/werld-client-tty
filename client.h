#ifndef WERLD_CLIENT_H
#define WERLD_CLIENT_H

#include "player.h"

#define WERLD_PLAYER_MESSAGE_BUFSIZ (80)

int client_connect(struct player);
int client_disconnect(struct player);
int client_send_player(struct player);
int client_send_message(struct player, const char *);
int client_request_players(void);
int client_handle_response(void);

#endif
