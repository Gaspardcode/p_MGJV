# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` 
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: main image

SRC = image.c 
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=} 

main: image.o
image: image.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
