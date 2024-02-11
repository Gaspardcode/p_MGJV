# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -march=native `pkg-config --cflags sdl2 SDL2_image` \
`pkg-config --cflags gtk+-3.0` -g
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image` -lm -fPIE

SRC = main.c image.c masks.c graphics.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: graphics

main: main.o masks.o image.o
graphics: graphics.o image.o 

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main graphics *.png
	${RM} ../make_solve/images/cases/*.bmp
	${RM} ../make_solve/images/caracters/*.bmp
	${RM} ../digit_recognition/ocr
	${RM} ../make_solve/include/character_detection/detection

# END
