CC      = gcc
CFLAGS  = -O2 -W -Wall -pedantic -std=c99 -g -D_GNU_SOURCE
LDFLAGS = -lncurses

EXECUTABLE  = werld_client
SRC_DIR     = .
SRC         = $(SRC_DIR)/main.c
SRC_OBJECTS = \
              $(SRC_DIR)/client.o       \
              $(SRC_DIR)/keyboard.o     \
              $(SRC_DIR)/movement.o     \
              $(SRC_DIR)/player.o       \
              $(SRC_DIR)/player_list.o  \
              $(SRC_DIR)/ui.o           \
              $(SRC_DIR)/werld_client.o \

ifeq ($(WERLD_ENV), development)
  CFLAGS += -DWERLD_DEVELOPMENT
endif

all: werld_client

client.o: client.c client.h player.h movement.h player_list.h ui.h
keyboard.o: keyboard.c client.h player.h movement.h player_list.h
main.o: main.c client.h player.h movement.h keyboard.h ui.h player_list.h
movement.o: movement.c movement.h
player.o: player.c client.h player.h movement.h ui.h player_list.h
player_list.o: player_list.c player_list.h player.h movement.h
ui.o: ui.c player.h movement.h player_list.h

werld_client: $(SRC_OBJECTS)
	$(CC) $(CFLAGS) $(SRC) -o $(EXECUTABLE) $(SRC_OBJECTS) $(LDFLAGS)

dep:
	$(CC) -MM $(SRC_DIR)/*.c

clean:
	rm -rf $(SRC_DIR)/*.o $(EXECUTABLE)
