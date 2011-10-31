#ifndef WERLD_CLIENT_WERLD_CLIENT_H
#define WERLD_CLIENT_WERLD_CLIENT_H

#include "player_list.h"

#define SERVER_ADDRESS "0.0.0.0"
#define SERVER_PORT    "9876"

#define EVENT_HEADER_BUFSIZ (4)
#define MESSAGE_BUFSIZ      (140)

// FIXME: s/REQ/REQUEST
#define REQ_PLAYER     "player"
#define REQ_PLAYERS    "players"
#define REQ_REGISTER   "register"
#define REQ_UNREGISTER "unregister"
#define REQ_MESSAGE    "message"

#define EVENT_ERROR   -1
#define EVENT_MESSAGE 0
#define EVENT_PLAYERS 1

#define RESPONSE_ERROR   -1
#define RESPONSE_MESSAGE 0

#define REQ_PLAYER_BUFSIZ     (strlen(REQ_PLAYER) + sizeof(struct player))
#define REQ_REGISTER_BUFSIZ   (strlen(REQ_REGISTER) + sizeof(struct player))
#define REQ_UNREGISTER_BUFSIZ (strlen(REQ_UNREGISTER) + sizeof(struct player))
#define REQ_MESSAGE_BUFSIZ    (strlen(REQ_MESSAGE) + sizeof(struct player) + MESSAGE_BUFSIZ)

#define RESPONSE_MESSAGE_BUFSIZ (sizeof(int) + sizeof(struct player) + MESSAGE_BUFSIZ)

int werld_client_connect(struct player);
void werld_client_disconnect(void);
int werld_client_send_player(struct player);
void werld_client_request_players(void);
int werld_client_handle_response(void);

int fd;

#endif
