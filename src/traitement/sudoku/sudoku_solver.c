#include <stdio.h>
#include <assert.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

int solvable_line(unsigned int *array)
{
	unsigned int value; 
	unsigned int line = 9; // line is the length of the square
	unsigned int loop = 0; // the loop is usefull to know 
			       // where we want to see in the array

	// LINE LOOP
	for(unsigned int column = 0; column < line; column++)
	{
		loop = column;
		for(unsigned int index = 0 + (line * loop); index 
				< (line * (loop+1)); index++)
		{

			//init the value
			//line * loop : the nth line
			value = *(array + index);
			//if the value is not already 
			//discovered we do not care about
			if(value != 0)
			{
				// know we need to conpare the 
				// value with the rest of the line, 
				// v will be the name of the other value
				for(unsigned int i = index+1;
						i < (line*(loop+1)); i++)
				{
					unsigned int v = *(array + i);
					if(v != 0 && v == value)
					{
						return 1;
					}
				}
			}
		}
	}



	return 0;

}

int solvable_column(unsigned int *array, unsigned int len)
{
	unsigned int column = 9;

	for(unsigned int column_index = 0; column_index 
			< column; column_index++)
	{
		for(unsigned int index = column_index; index 
				< len-(9-column_index); index+=column)
		{
			unsigned int value = *(array + index);
			if(value != 0)
			{
				for(unsigned int i = index+column; i 
						< len-(8-column_index); 
						i+=column)
				{
					unsigned int v = *(array + i);
					if(v != 0 && v == value)
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int square(unsigned int *array, unsigned int n)
{
	for(unsigned int i = n; i < n + 21;)
	{
		unsigned int index = i;
		unsigned int value = *(array + i);
		while(index < 20+n)
		{
			if(index + 1 == n+3 || index + 1 == n+12)
				index += 7;
			else
				index += 1;
			unsigned int v = *(array + index);
			if(v != 0 && v == value)
			{
				return 1;
			}
		}
		if(i + 1 == n+3 || i + 1 == n+12)
			i += 7;
		else
			i += 1;
	}
	return 0;
}

int solved_square(unsigned int *array)
{
	for(unsigned int i = 0; i < 61;)
	{
		if(square(array, i) != 0)
			return 1;
		if(i + 3 == 9 || i + 3 == 36)
			i += 21;
		else
			i += 3;
	}
	return 0;
}

int sudoku_auth(unsigned int *array, unsigned int len)
{
	int c = solvable_line(array);
	if(c == 1)
		return 1;
	int c2 = solvable_column(array, len);
	if(c2 == 1)
		return 1;
	int s = solved_square(array);
	if(s == 1)
		return 1;
	return 0;
}
