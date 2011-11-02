#ifndef WERLD_CLIENT_SERVER_H
#define WERLD_CLIENT_SERVER_H

#include "player.h"

#define WERLD_SERVER_ADDRESS "0.0.0.0"
#define WERLD_SERVER_PORT    "9876"

#define MESSAGE_BUFSIZ      (140)

#define REQUEST_PLAYER     "player"
#define REQUEST_PLAYERS    "players"
#define REQUEST_REGISTER   "register"
#define REQUEST_UNREGISTER "unregister"
#define REQUEST_MESSAGE    "message"

#define EVENT_HEADER_BUFSIZ (4)

#define EVENT_ERROR   -1
#define EVENT_MESSAGE 0
#define EVENT_PLAYERS 1

#define RESPONSE_ERROR   -1
#define RESPONSE_MESSAGE 0

#define REQUEST_PLAYER_BUFSIZ     (strlen(REQUEST_PLAYER) + \
                                   sizeof(struct player))
#define REQUEST_REGISTER_BUFSIZ   (strlen(REQUEST_REGISTER) + \
                                   sizeof(struct player))
#define REQUEST_UNREGISTER_BUFSIZ (strlen(REQUEST_UNREGISTER) + \
                                   sizeof(struct player))
#define REQUEST_MESSAGE_BUFSIZ    (strlen(REQUEST_MESSAGE) + \
                                   sizeof(struct player) + \
                                   MESSAGE_BUFSIZ)

#define RESPONSE_MESSAGE_BUFSIZ (sizeof(int) + \
                                 sizeof(struct player) + \
                                 MESSAGE_BUFSIZ)

extern int fd;

int client_connect(struct player);
void client_disconnect(struct player);
int client_send_player(struct player);
void client_send_message(struct player, const char *);
void client_request_players(void);
int client_handle_response(void);

#endif
