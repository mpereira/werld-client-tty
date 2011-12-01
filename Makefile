CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99 -D_GNU_SOURCE
LDFLAGS = -lncurses

EXECUTABLE  = werld_client
SRC_DIR     = .
SRC         = $(SRC_DIR)/main.c
SRC_OBJECTS = \
              $(SRC_DIR)/client.o          \
              $(SRC_DIR)/keyboard.o        \
              $(SRC_DIR)/map.o             \
              $(SRC_DIR)/maps.o            \
              $(SRC_DIR)/message.o         \
              $(SRC_DIR)/message_bar.o     \
              $(SRC_DIR)/message_handler.o \
              $(SRC_DIR)/message_list.o    \
              $(SRC_DIR)/movement.o        \
              $(SRC_DIR)/net.o             \
              $(SRC_DIR)/player.o          \
              $(SRC_DIR)/player_list.o     \
              $(SRC_DIR)/request_type.o    \
              $(SRC_DIR)/status_bar.o      \
              $(SRC_DIR)/tty.o             \
              $(SRC_DIR)/ui.o              \
              $(SRC_DIR)/werld_client.o    \
              $(SRC_DIR)/window.o          \

ifeq ($(WERLD_ENV), development)
  CFLAGS += -g -DWERLD_DEVELOPMENT
else
  CFLAGS += -O3
endif

all: werld_client

client.o: client.c client.h player.h movement.h message_handler.h net.h \
 player_list.h request_type.h message.h ui.h message_list.h \
 werld_client.h
keyboard.o: keyboard.c client.h player.h movement.h message.h \
 message_bar.h status_bar.h tty.h ui.h message_list.h player_list.h \
 werld_client.h window.h
main.o: main.c client.h player.h movement.h keyboard.h message_bar.h \
 message_handler.h player_list.h status_bar.h tty.h werld_client.h \
 window.h
message_bar.o: message_bar.c message.h message_bar.h
message.o: message.c
message_handler.o: message_handler.c message_handler.h player.h \
 movement.h message_list.h net.h werld_client.h player_list.h ui.h
message_list.o: message_list.c message.h message_list.h
movement.o: movement.c movement.h
net.o: net.c
player.o: player.c client.h player.h movement.h ui.h message_list.h \
 player_list.h
player_list.o: player_list.c message_list.h player_list.h player.h \
 movement.h ui.h
request_type.o: request_type.c
status_bar.o: status_bar.c status_bar.h player.h movement.h
tty.o: tty.c tty.h ui.h message_list.h player.h movement.h player_list.h \
 werld_client.h
ui.o: ui.c message_list.h player.h movement.h player_list.h \
 werld_client.h
werld_client.o: werld_client.c werld_client.h player.h movement.h \
 player_list.h
window.o: window.c window.h

werld_client: $(SRC_OBJECTS)
	$(CC) $(CFLAGS) $(SRC) -o $(EXECUTABLE) $(SRC_OBJECTS) $(LDFLAGS)

dep:
	$(CC) -MM $(SRC_DIR)/*.c

clean:
	rm -rf $(SRC_DIR)/*.o $(EXECUTABLE)
