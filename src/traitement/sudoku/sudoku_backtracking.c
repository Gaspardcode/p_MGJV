#include <stdio.h>
int emptyCellCheck(unsigned int *array, unsigned int len)
{
	for(unsigned int i = 0; i < len; i++)
	{
		if(array[i] == 0)
		{
			return 1;
		}
	}
	return 0;
}

int rowCheck(unsigned int *array, unsigned int row, unsigned int value)
{
	//this function is used to know if a Value can be placed at 
	//the array[column + row*9] place

	for(unsigned int col = 0; col < 9; col++)
	{
		if(value == array[col + row*9])
		{
			return 1;
		}
	}
	return 0;
}

int columnCheck(unsigned int *array, unsigned int column, unsigned int value)
{
	for(unsigned int row = 0; row < 9; row++)
	{
		if(value == array[column + row*9])
		{
			return 1;
		}
	}
	return 0;
}

int squareCheck(unsigned int *array, unsigned int squareRow, 
		unsigned int squareColumn, unsigned int value)
{
	for(unsigned int i = 0; i < 3; i++)
	{
		for(unsigned int j = 0; j < 3; j++)
		{
			if(array[(squareColumn+i) + (squareRow+j)*9] == value)
			{
				return 1;
			}
		}
	}
	return 0;
}


int allCheck(unsigned int *array, unsigned int row, unsigned int column, 
		unsigned int value)
{
	int squareRow = row - row%3;
	int squareColumn = column -column%3;
	int rc = rowCheck(array, row, value);
	int cc = columnCheck(array, column, value);
	int sc = squareCheck(array, squareRow, squareColumn, value);

	if(rc + cc + sc != 0)
		return 1;
	return 0;
}


int solveSudo(unsigned int *array, unsigned int row, unsigned int column,
		unsigned int len)
{
	if( row == len-1 && column == len)
		return 1;

	if(column == len)
	{
		row++;
		column = 0;
	}

	if(array[column + row*len] != 0)
		return solveSudo(array, row, column+1, len);


	for(unsigned int n = 1; n <= len; n++)
	{
		if(allCheck(array, row, column, n) == 0)
		{
			*(array + row*len + column) = n;

			if(solveSudo(array, row, column+1, len) == 1)
				return 1;
		}
			*(array+row*len + column) = 0;
	}

	return 0;



}


