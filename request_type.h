#ifndef WERLD_REQUEST_TYPE_H
#define WERLD_REQUEST_TYPE_H

#include <stddef.h>
#include <stdint.h>

extern const uint8_t WERLD_REQUEST_TYPE_PLAYER;
extern const uint8_t WERLD_REQUEST_TYPE_PLAYERS;
extern const uint8_t WERLD_REQUEST_TYPE_REGISTER;
extern const uint8_t WERLD_REQUEST_TYPE_UNREGISTER;
extern const uint8_t WERLD_REQUEST_TYPE_MESSAGE;
extern const uint8_t WERLD_REQUEST_TYPE_MAP;

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

#define WERLD_REQUEST_TYPE_MAP_PACKET_SIZE (WERLD_REQUEST_TYPE_BUFSIZ + \
                                            WERLD_MAPS_HEADER_FIELD_SIZE)

#endif
