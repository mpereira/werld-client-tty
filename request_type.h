#ifndef WERLD_REQUEST_TYPE_H
#define WERLD_REQUEST_TYPE_H

#include <stdint.h>
#include <string.h>

#include "message.h"
#include "player.h"

extern const uint8_t WERLD_REQUEST_TYPE_PLAYER;
extern const uint8_t WERLD_REQUEST_TYPE_PLAYERS;
extern const uint8_t WERLD_REQUEST_TYPE_REGISTER;
extern const uint8_t WERLD_REQUEST_TYPE_UNREGISTER;
extern const uint8_t WERLD_REQUEST_TYPE_MESSAGE;

extern const size_t WERLD_REQUEST_TYPE_BUFSIZ;

#define WERLD_REQUEST_TYPE_PLAYER_BUFSIZ (WERLD_REQUEST_TYPE_BUFSIZ + \
                                          sizeof(struct player))

#define WERLD_REQUEST_TYPE_REGISTER_BUFSIZ (WERLD_REQUEST_TYPE_BUFSIZ + \
                                            sizeof(struct player))

#define WERLD_REQUEST_TYPE_UNREGISTER_BUFSIZ (WERLD_REQUEST_TYPE_BUFSIZ + \
                                              sizeof(struct player))

#define WERLD_REQUEST_TYPE_MESSAGE_BUFSIZ(message) (WERLD_REQUEST_TYPE_BUFSIZ + \
                                                    sizeof(struct player) + \
                                                    strlen(message))

#endif
