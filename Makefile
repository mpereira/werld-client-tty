CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99 -D_GNU_SOURCE
LDFLAGS = -lncurses

EXECUTABLE  = werld
SRC_DIR     = .
SRC         = $(SRC_DIR)/main.c
SRC_OBJECTS = \
              $(SRC_DIR)/client.o             \
              $(SRC_DIR)/keyboard.o           \
              $(SRC_DIR)/main_window.o        \
              $(SRC_DIR)/map.o                \
              $(SRC_DIR)/maps.o               \
              $(SRC_DIR)/message.o            \
              $(SRC_DIR)/message_bar_window.o \
              $(SRC_DIR)/message_handler.o    \
              $(SRC_DIR)/message_list.o       \
              $(SRC_DIR)/movement.o           \
              $(SRC_DIR)/net.o                \
              $(SRC_DIR)/player.o             \
              $(SRC_DIR)/player_list.o        \
              $(SRC_DIR)/request_type.o       \
              $(SRC_DIR)/status_bar_window.o  \
              $(SRC_DIR)/tile.o               \
              $(SRC_DIR)/tty.o                \
              $(SRC_DIR)/ui.o                 \
              $(SRC_DIR)/werld_client.o       \

PREFIX  = /usr/local
BIN_DIR = $(PREFIX)/bin
INSTALL = cp -p

ifeq ($(WERLD_ENV), development)
  CFLAGS += -O1 -g3 -DWERLD_DEVELOPMENT
else
  CFLAGS += -O3
endif

all: $(EXECUTABLE)

werld: $(SRC_OBJECTS)
	$(CC) $(CFLAGS) $(SRC) -o $(EXECUTABLE) $(SRC_OBJECTS) $(LDFLAGS)

install: all
	mkdir -p $(BIN_DIR)
	$(INSTALL) $(EXECUTABLE) $(BIN_DIR)

dep:
	$(CC) -MM $(SRC_DIR)/*.c

clean:
	rm -rf $(SRC_DIR)/*.o $(EXECUTABLE)
