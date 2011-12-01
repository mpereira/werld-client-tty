#ifndef WERLD_CLIENT_H
#define WERLD_CLIENT_H

#include <stdint.h>

#include "player.h"

int client_request_connect(struct player);
int client_request_disconnect(struct player *);
int client_request_player(struct player);
int client_request_message(struct player, const char *);
int client_request_players(void);
int client_request_map(uint8_t);
int client_handle_response(void);

#endif
