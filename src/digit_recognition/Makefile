# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -march=native `pkg-config --cflags sdl2 SDL2_image` \
`pkg-config --cflags gtk+-3.0` -g
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl2 SDL2_image` -lm -fPIE

SRC = image.c masks.c dataset.c knn.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all:dataset knn 

dataset:dataset.o image.o
knn: knn.o image.o

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main knn dataset data/*/[0-9][0-9][0-9][0-9][0-9][0-9].png
	${RM} ../make_solve/images/cases/*.bmp
	${RM} ../make_solve/images/caracters/*.bmp
	${RM} ../digit_recognition/ocr
	${RM} ../make_solve/include/character_detection/detection

# END
