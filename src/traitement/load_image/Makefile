# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: load_image

SRC = load_image.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

load_image = load_image.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
