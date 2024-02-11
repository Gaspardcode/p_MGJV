# Compiler and linker settings
CC = gcc
CFLAGS = -Wall `pkg-config --cflags sdl2 SDL2_image` -g
LDFLAGS = -lSDL2 -lm -lSDL2_image
LDLIBS = `pkg-config --libs sdl2 SDL2_image`


# Source files and target executable
SRC = ocrmnist.c
OBJS = $(SRC:.c=.o)
TARGET = ocr

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

delete:
	rm -f *.txt
	touch hweight.txt
	touch obias.txt
	touch oweight.txt
	touch hbias.txt
	touch hweight2.txt
	touch obias2.txt
	touch oweight2.txt
	touch hbias2.txt

