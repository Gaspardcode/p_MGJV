# ifndef _SUDOKU_BACKTRACKING_H
# define _SUDOKU_BACKTRACKING_H

int emptyCellCheck(unsigned int *array, unsigned int len);
int rowCheck(unsigned int *array, unsigned int row, unsigned int value);
int columnCheck(unsigned int *array, unsigned int column, unsigned int value);
int squareCheck(unsigned int *array, unsigned int squareRow, unsigned int squareColumn , unsigned int value);

int allCheck(unsigned int *array, unsigned int row, unsigned int column, unsigned int value);

int solveSudo(unsigned int *array, unsigned int row, unsigned int column, unsigned int len);

# endif
