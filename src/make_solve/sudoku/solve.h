# ifndef SOLVE_H
# define SOLVE_H

#include "sudoku_solver.h"
#include "sudoku_backtracking.h"
#include "err.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

typedef struct
{
	int id;
	int ra_num;
}digit;
int cmpfunc (const void *a, const void *b)
{
	return (((digit*)a)->ra_num - ((digit*)b)->ra_num);
}
int seed = 46;
void ran()
{
	int a = 2415245245;
	int b = 1;
	int m = 456789;
	seed = seed*a + b % m;
}
void file_to_array(char path[],unsigned int *array )
{
	FILE *file;
	file = fopen(path, "r");

	char c = fgetc(file);

	int index = 0;
	while(c != EOF)
	{
		if(c == '.')
		{
			array[index] = 0;
			index++;
		}
		else if (c <= '9' && c >= '0' )
		{
			array[index] = c - '0';
			index++;
		}

		c = fgetc(file);

	}
	fclose(file);

}
void draw_grid(SDL_Surface * img)
{
	// Draws the grid of the sudoku on the surface
	// Size is to be modified to get bigger grid
	printf("drawing grid\n");
	int w = img->w;
	int size = 5;
	SDL_Surface * hor = SDL_CreateRGBSurface(0,w,size,32,0,0,0,0);
	SDL_Surface * ver = SDL_CreateRGBSurface(0,size,w,32,0,0,0,0);
	SDL_FillRect(hor,NULL,0xFF000000);
	SDL_FillRect(ver,NULL,0xFF000000);

	for(int i = 0; i < w; i+=100)
	{
		SDL_Rect dst = {0,i,w,size};
		SDL_BlitSurface(hor,NULL,img,&dst);
	}
	SDL_Rect dsti = {0,w-size,w,size};
	SDL_BlitSurface(hor,NULL,img,&dsti);
	for(int i = 0; i < w; i+=100)
	{
		SDL_Rect dst = {i,0,size,w};
		SDL_BlitSurface(ver,NULL,img,&dst);
	}
	SDL_Rect dsto = {w-size,0,size,w};
	SDL_BlitSurface(ver,NULL,img,&dsto);

	printf("ended\n");
}
void draw_square(SDL_Surface * img, int x, int y)
{
	int w = img->w;
	int size = 5;
	// B for border
	w /= 9;
	SDL_Surface * hor = SDL_CreateRGBSurface(0,w,size,32,0,0,0,0);
	SDL_Surface * ver = SDL_CreateRGBSurface(0,size,w,32,0,0,0,0);
	SDL_FillRect(hor,NULL,0xFF000000);
	SDL_FillRect(ver,NULL,0xFF000000);

	int adjx = 0;
	int adjy = 0;
	if(x == w*8)
		adjx = -size;
	if(y == w*8)
		adjy = -size;
	SDL_Rect dsta = {y,x,w,size};
	SDL_BlitSurface(hor,NULL,img,&dsta);
	SDL_Rect dstb = {y,x+w+adjx,w,size};
	SDL_BlitSurface(hor,NULL,img,&dstb);

	SDL_Rect dstc = {y,x,size,w};
	SDL_BlitSurface(ver,NULL,img,&dstc);
	SDL_Rect dstd = {y+w+adjy,x,size,w};
	SDL_BlitSurface(ver,NULL,img,&dstd);

}
void draw_cross(SDL_Surface * img)
{
	// Draws the grid of the sudoku on the surface
	// Size is to be modified to get bigger grid
	printf("drawing cross\n");
	int w = img->w;
	int size = 10;
	SDL_Surface * hor = SDL_CreateRGBSurface(0,w,size,32,0,0,0,0);
	SDL_Surface * ver = SDL_CreateRGBSurface(0,size,w,32,0,0,0,0);
	SDL_FillRect(hor,NULL,0xFF000000);
	SDL_FillRect(ver,NULL,0xFF000000);

	for(int i = (w/3); i < w; i+=(w/3))
	{
		SDL_Rect dst = {0,i,w,size};
		SDL_BlitSurface(hor,NULL,img,&dst);
	}
	for(int i = (w/3); i < w; i+=(w/3))
	{
		SDL_Rect dst = {i,0,size,w};
		SDL_BlitSurface(ver,NULL,img,&dst);
	}
	printf("ended\n");
}

void array_to_file(char path[],unsigned int *array )
{

	FILE *file;
	file = fopen(path, "w");
	if(file == NULL)
	{
		errx(1, "fopen()");
	}


	for(int i = 0; i < 81; i++)
	{
		if(i%3 == 0 && i%9 != 0)
			fprintf(file, " ");
		if(i%27 == 0 && i!=0)
			fprintf(file, "\n");
		if(i%9 != 8)
			fprintf(file ,"%i", *(array + i));
		else
			fprintf(file, "%i\n", *(array + i));
	}


	fclose(file);

}

void sudoku_to_img(unsigned int * before, unsigned int * solved)
{
	int size = 100;
	int x,y;
	SDL_Surface * sco = IMG_Load("../make_solve/sudoku/digits/blank.png");
	if(sco == NULL)
		printf("sco NULL 1\n");

	//loading digits
	SDL_Surface * B1 = IMG_Load("../make_solve/sudoku/digits/1B.png");
	SDL_Surface * G1 = IMG_Load("../make_solve/sudoku/digits/1G.png");
	SDL_Surface * B2 = IMG_Load("../make_solve/sudoku/digits/2B.png");
	SDL_Surface * G2 = IMG_Load("../make_solve/sudoku/digits/2G.png");
	SDL_Surface * B3 = IMG_Load("../make_solve/sudoku/digits/3B.png");
	SDL_Surface * G3 = IMG_Load("../make_solve/sudoku/digits/3G.png");
	SDL_Surface * B4 = IMG_Load("../make_solve/sudoku/digits/4B.png");
	SDL_Surface * G4 = IMG_Load("../make_solve/sudoku/digits/4G.png");
	SDL_Surface * B5 = IMG_Load("../make_solve/sudoku/digits/5B.png");
	SDL_Surface * G5 = IMG_Load("../make_solve/sudoku/digits/5G.png");
	SDL_Surface * B6 = IMG_Load("../make_solve/sudoku/digits/6B.png");
	SDL_Surface * G6 = IMG_Load("../make_solve/sudoku/digits/6G.png");
	SDL_Surface * B7 = IMG_Load("../make_solve/sudoku/digits/7B.png");
	SDL_Surface * G7 = IMG_Load("../make_solve/sudoku/digits/7G.png");
	SDL_Surface * B8 = IMG_Load("../make_solve/sudoku/digits/8B.png");
	SDL_Surface * G8 = IMG_Load("../make_solve/sudoku/digits/8G.png");
	SDL_Surface * B9 = IMG_Load("../make_solve/sudoku/digits/9B.png");
	SDL_Surface * G9 = IMG_Load("../make_solve/sudoku/digits/9G.png");

	int len = 81;
	const SDL_Rect src = {0,0,size,size};
	for(int i = 0; i < len;i++)
	{
		if(before[i] != 0)
		{
			// marking given digits
			solved[i] = 0;
			x = (i/9)*size;
			y = (i%9)*size;

			//cropping digit in the right spot
			SDL_Rect dst = {y,x,size,size};
			switch(before[i])
			{
				case 1:
					SDL_BlitSurface(B1,&src,sco,&dst);
					break;
				case 2:
					SDL_BlitSurface(B2,&src,sco,&dst);
					break;
				case 3:
					SDL_BlitSurface(B3,&src,sco,&dst);
					break;
				case 4:
					SDL_BlitSurface(B4,&src,sco,&dst);
					break;
				case 5:
					SDL_BlitSurface(B5,&src,sco,&dst);
					break;
				case 6:
					SDL_BlitSurface(B6,&src,sco,&dst);
					break;
				case 7:
					SDL_BlitSurface(B7,&src,sco,&dst);
					break;
				case 8:
					SDL_BlitSurface(B8,&src,sco,&dst);
					break;
				case 9:
					SDL_BlitSurface(B9,&src,sco,&dst);
					break;
			}
		}
	}	

	//placing digits solved
	for(int i = 0; i < len;i++)
	{
		//getting solved digits
		if(solved[i] != 0)
		{
			x = (i/9)*size;
			y = (i%9)*size;

			//cropping digit in the right spot
			SDL_Rect dst = {y,x,size,size};
			switch(solved[i])
			{
				case 1:
					SDL_BlitSurface(G1,&src,sco,&dst);
					break;
				case 2:
					SDL_BlitSurface(G2,&src,sco,&dst);
					break;
				case 3:
					SDL_BlitSurface(G3,&src,sco,&dst);
					break;
				case 4:
					SDL_BlitSurface(G4,&src,sco,&dst);
					break;
				case 5:
					SDL_BlitSurface(G5,&src,sco,&dst);
					break;
				case 6:
					SDL_BlitSurface(G6,&src,sco,&dst);
					break;
				case 7:
					SDL_BlitSurface(G7,&src,sco,&dst);
					break;
				case 8:
					SDL_BlitSurface(G8,&src,sco,&dst);
					break;
				case 9:
					SDL_BlitSurface(G9,&src,sco,&dst);
					break;
			}
		}
	}	

	//draw_grid(sco);
	//save img
	if(sco != NULL)
		IMG_SavePNG(sco,"result.png");
	SDL_FreeSurface(sco);
	SDL_FreeSurface(B1);
	SDL_FreeSurface(G1);
	SDL_FreeSurface(B2);
	SDL_FreeSurface(G2);
	SDL_FreeSurface(B3);
	SDL_FreeSurface(G3);
	SDL_FreeSurface(B4);
	SDL_FreeSurface(G4);
	SDL_FreeSurface(B5);
	SDL_FreeSurface(G5);
	SDL_FreeSurface(B6);
	SDL_FreeSurface(G6);
	SDL_FreeSurface(B7);
	SDL_FreeSurface(G7);
	SDL_FreeSurface(B8);
	SDL_FreeSurface(G8);
	SDL_FreeSurface(B9);
	SDL_FreeSurface(G9);
}

void sudoku_to_all_steps_img(unsigned int * before, unsigned int * solved)
{
	int size = 100;
	int x,y;
	SDL_Surface * sco = IMG_Load("../make_solve/sudoku/digits/blank.png");
	if(sco == NULL)
		printf("sco NULL\n");

	//loading digits
	SDL_Surface * B1 = IMG_Load("../make_solve/sudoku/digits/1B.png");
	SDL_Surface * G1 = IMG_Load("../make_solve/sudoku/digits/1G.png");
	SDL_Surface * B2 = IMG_Load("../make_solve/sudoku/digits/2B.png");
	SDL_Surface * G2 = IMG_Load("../make_solve/sudoku/digits/2G.png");
	SDL_Surface * B3 = IMG_Load("../make_solve/sudoku/digits/3B.png");
	SDL_Surface * G3 = IMG_Load("../make_solve/sudoku/digits/3G.png");
	SDL_Surface * B4 = IMG_Load("../make_solve/sudoku/digits/4B.png");
	SDL_Surface * G4 = IMG_Load("../make_solve/sudoku/digits/4G.png");
	SDL_Surface * B5 = IMG_Load("../make_solve/sudoku/digits/5B.png");
	SDL_Surface * G5 = IMG_Load("../make_solve/sudoku/digits/5G.png");
	SDL_Surface * B6 = IMG_Load("../make_solve/sudoku/digits/6B.png");
	SDL_Surface * G6 = IMG_Load("../make_solve/sudoku/digits/6G.png");
	SDL_Surface * B7 = IMG_Load("../make_solve/sudoku/digits/7B.png");
	SDL_Surface * G7 = IMG_Load("../make_solve/sudoku/digits/7G.png");
	SDL_Surface * B8 = IMG_Load("../make_solve/sudoku/digits/8B.png");
	SDL_Surface * G8 = IMG_Load("../make_solve/sudoku/digits/8G.png");
	SDL_Surface * B9 = IMG_Load("../make_solve/sudoku/digits/9B.png");
	SDL_Surface * G9 = IMG_Load("../make_solve/sudoku/digits/9G.png");

	int len = 81;
	const SDL_Rect src = {0,0,size,size};
	int step_done = 0;
	for(int i = 0; i < len;i++)
	{
		if(before[i] != 0)
		{
			step_done++;
			// marking given digits
			solved[i] = 0;
			x = (i/9)*size;
			y = (i%9)*size;

			//cropping digit in the right spot
			SDL_Rect dst = {y,x,size,size};
			switch(before[i])
			{
				case 1:
					SDL_BlitSurface(B1,&src,sco,&dst);
					break;
				case 2:
					SDL_BlitSurface(B2,&src,sco,&dst);
					break;
				case 3:
					SDL_BlitSurface(B3,&src,sco,&dst);
					break;
				case 4:
					SDL_BlitSurface(B4,&src,sco,&dst);
					break;
				case 5:
					SDL_BlitSurface(B5,&src,sco,&dst);
					break;
				case 6:
					SDL_BlitSurface(B6,&src,sco,&dst);
					break;
				case 7:
					SDL_BlitSurface(B7,&src,sco,&dst);
					break;
				case 8:
					SDL_BlitSurface(B8,&src,sco,&dst);
					break;
				case 9:
					SDL_BlitSurface(B9,&src,sco,&dst);
					break;
			}
		}
	}	

	draw_grid(sco);
	draw_cross(sco);
	if(sco != NULL)
		IMG_SavePNG(sco,"empty_grid.png");

	char path[11] = "step__.png";
	// Get the digits to fit in the grid, cropping them smaller
	int crop = 10;
	const SDL_Rect srca = {crop,crop,size-crop,size-crop};

	// Generating a sequence of number from 1 to step_to_do
	// In a random order
	int step_to_do = 81 - step_done;
	digit steps[step_to_do];
	int pos = -1;
	for(int i = 0;i < step_to_do; i++)
	{
		//Seeks the position of the number in the array solved 
		//to later get the coordinates
		while(!solved[++pos]);
		steps[i].id = pos;
		ran();
		steps[i].ra_num = seed; //(int)rand();
	}

	qsort(steps,step_to_do,sizeof(digit),cmpfunc);

	//placing digits solved
	for(int step = 0; step < step_to_do; step++)
	{
		int i = steps[step].id;
		x = (i/9)*size;
		y = (i%9)*size;

		//cropping digit in the right spot
		SDL_Rect dst = {y+crop,x+crop,size-crop,size-crop};
		switch(solved[i])
		{
			case 1:
				SDL_BlitSurface(G1,&srca,sco,&dst);
				break;
			case 2:
				SDL_BlitSurface(G2,&srca,sco,&dst);
				break;
			case 3:
				SDL_BlitSurface(G3,&srca,sco,&dst);
				break;
			case 4:
				SDL_BlitSurface(G4,&srca,sco,&dst);
				break;
			case 5:
				SDL_BlitSurface(G5,&srca,sco,&dst);
				break;
			case 6:
				SDL_BlitSurface(G6,&srca,sco,&dst);
				break;
			case 7:
				SDL_BlitSurface(G7,&srca,sco,&dst);
				break;
			case 8:
				SDL_BlitSurface(G8,&srca,sco,&dst);
				break;
			case 9:
				SDL_BlitSurface(G9,&srca,sco,&dst);
				break;
		}
		//saving step by step solution
		path[4] = step / 10 + '0';
		path[5] = step % 10 + '0';
		// Re-draw the grid around the digit
		draw_square(sco,x,y);
		if(sco != NULL)
			IMG_SavePNG(sco,path);
	}	

	//save img
	if(sco != NULL)
		IMG_SavePNG(sco,"result.png");
	SDL_FreeSurface(sco);
	SDL_FreeSurface(B1);
	SDL_FreeSurface(G1);
	SDL_FreeSurface(B2);
	SDL_FreeSurface(G2);
	SDL_FreeSurface(B3);
	SDL_FreeSurface(G3);
	SDL_FreeSurface(B4);
	SDL_FreeSurface(G4);
	SDL_FreeSurface(B5);
	SDL_FreeSurface(G5);
	SDL_FreeSurface(B6);
	SDL_FreeSurface(G6);
	SDL_FreeSurface(B7);
	SDL_FreeSurface(G7);
	SDL_FreeSurface(B8);
	SDL_FreeSurface(G8);
	SDL_FreeSurface(B9);
	SDL_FreeSurface(G9);
}

int solve(char *arg)
{
	unsigned int len = 9*9;
	unsigned int * solved = calloc(len, sizeof (unsigned int));
	unsigned int * before = calloc(len, sizeof (unsigned int));

	file_to_array(arg, solved);
	file_to_array(arg, before);

	int err = solveSudo(solved, 0, 0, 9);

	if(err == 0)
		printf("fail");
	else
	{
		array_to_file("../make_solve/sudoku/grids/grid_00.result", solved);
		//sudoku_to_img(before,solved);
		//Uncomment below to see all step img
		sudoku_to_all_steps_img(before,solved);
	}

	free(solved);
	free(before);
	return 0;

}

# endif
