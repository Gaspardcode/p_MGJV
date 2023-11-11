# ifndef _SUDOKU_SOLVER_H
# define _SUDOKU_SOLVER_H

#include <stdio.h>
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

int solvable_line(unsigned int *array, unsigned int len);
int solvable_column(unsigned int *array, unsigned int len);
int square(unsigned int *array, unsigned int n, unsigned int len);
int solved_square(unsigned int *array, unsigned int len);
int sudoku_auth(unsigned int *array, unsigned int len);

# endif
