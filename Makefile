CC      = gcc
CFLAGS  = -O2 -W -Wall -pedantic -ansi -std=c99 -g
LDFLAGS = -lncurses

EXECUTABLE = werld_client
SRC_DIR    = .
SRC        = ${SRC_DIR}/werld_client.c
SRC_OBJECTS = ${SRC_DIR}/keyboard.o \
              ${SRC_DIR}/ui.o       \
              ${SRC_DIR}/movement.o \
              ${SRC_DIR}/player.o

all: werld_client

werld_client: ${SRC_OBJECTS}
	${CC} ${CFLAGS} ${SRC} -o ${EXECUTABLE} ${SRC_OBJECTS} ${LDFLAGS}

clean:
	rm -rf ${SRC_DIR}/*.o ${EXECUTABLE}
