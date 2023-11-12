CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99 -march=native -g `pkg-config --cflags sdl2 SDL2_image`
SOURCES = sudoku_backtracking.c sudoku_solver.c main.c
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = solver
OUTPUT = result.png empty_grid.png step* grids/grid_00.result

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(OUTPUT) 
