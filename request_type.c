#include <stddef.h>
#include <stdint.h>

const uint8_t WERLD_REQUEST_TYPE_PLAYER     = 0;
const uint8_t WERLD_REQUEST_TYPE_PLAYERS    = 1;
const uint8_t WERLD_REQUEST_TYPE_REGISTER   = 2;
const uint8_t WERLD_REQUEST_TYPE_MESSAGE    = 3;
const uint8_t WERLD_REQUEST_TYPE_MAP        = 4;

const size_t WERLD_REQUEST_TYPE_BUFSIZ = sizeof(uint8_t);
