# Automatic compilation
CC = gcc
CFLAGS = -Isrc -Wall -Wextra -Werror -O3 -std=c99 -march=native -I/usr/include/SDL -lSDL -lSDL_image 
FILES = $(shell find . -name '*.c')
NAME = main

all: $(NAME)

show:
	@echo $(FILES)

main:	$(FILES) 
	$(CC) $< $(CFLAGS) -o $@
	@echo project built successfully
clean:
	rm -rf main
	@echo "cleaning done"
