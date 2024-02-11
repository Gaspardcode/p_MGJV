#ifndef SEND_SOLUTION_H
#define SEND_SOLUTION_H
#include "../sudoku/solve.h"
#include "../include/epurator/epurate.h"
#include "../include/separator/separate.h"
#include <err.h>
#include "../include/recognition/ocr.h"
#include "../include/recognition/load_image.h"
#include "../include/character_detection/detection.h"
#include <stdio.h>
#include "../sudoku/solve.h"

int send_solution(char* input_file)
{
	printf("Send solution of %s\n", input_file);

	separate(input_file);
	printf("Successful separation\n");

	epure(0);

	printf("Successful epuration\n");
	int* sudoku = calloc(81, sizeof(int));
	printf("Going to prepare\n");
	prepare(sudoku);
	printf("Successful preparation");



	FILE *file = fopen("../make_solve/sudoku/grids/grid_00", "w");
	if(file == NULL)
	{
		perror("Error opening file");
		return 1;
	}

	int len = 9*9;
	int *begin = sudoku, *end = sudoku + len;

	for(int i = 0; begin+i < end; i++)
	{
		if(i%3 == 0 && i%9 != 0)
			fprintf(file, " ");
		if(i%27 == 0 && i!=0)
			fprintf(file, "\n");
		if(i%9 != 8)
		{
			if(*(sudoku +  i) != 0)
				fprintf(file, "%i", *(sudoku + i));
			else
				fprintf(file, ".");
		}
		else
		{
			if(*(sudoku + i) != 0)
				fprintf(file, "%i\n", *(sudoku + i));
			else
				fprintf(file, ".\n");
		}
	}

	fclose(file);
	free(sudoku);
	solve("../make_solve/sudoku/grids/grid_00");
	return 0;
}

# endif
