#ifndef WERLD_CLIENT_WERLD_CLIENT_H
#define WERLD_CLIENT_WERLD_CLIENT_H

#include "player_list.h"

#define SERVER_ADDRESS "0.0.0.0"
#define SERVER_PORT "9876"

#define GET_PLAYERS "get_players"

#define SEND_PLAYER_BUFFER_SIZE 1
#define GET_PLAYERS_BUFFER_SIZE (1024 * 1024)

int werld_client_connect(void);
void werld_client_disconnect(void);
int werld_client_send_player(struct player);
struct player_list *werld_client_get_players(void);

int fd;

#endif
