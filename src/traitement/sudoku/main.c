#include "sudoku_solver.h"
#include "sudoku_backtracking.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

void array_to_file(char path[],unsigned int *array )
{
	FILE *file;
	file = fopen(path, "w");

	int len = 9*9;
	unsigned int *begin = array, *end = array + len;

	for(int i = 0; begin+i < end; i++)
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
	SDL_Surface * sco = IMG_Load("digits/blank.png");
	if(sco == NULL)
		printf("sco NULL\n");

	//loading digits
	SDL_Surface * B1 = IMG_Load("digits/1B.png");
	SDL_Surface * G1 = IMG_Load("digits/1G.png");
	SDL_Surface * B2 = IMG_Load("digits/2B.png");
	SDL_Surface * G2 = IMG_Load("digits/2G.png");
	SDL_Surface * B3 = IMG_Load("digits/3B.png");
	SDL_Surface * G3 = IMG_Load("digits/3G.png");
	SDL_Surface * B4 = IMG_Load("digits/4B.png");
	SDL_Surface * G4 = IMG_Load("digits/4G.png");
	SDL_Surface * B5 = IMG_Load("digits/5B.png");
	SDL_Surface * G5 = IMG_Load("digits/5G.png");
	SDL_Surface * B6 = IMG_Load("digits/6B.png");
	SDL_Surface * G6 = IMG_Load("digits/6G.png");
	SDL_Surface * B7 = IMG_Load("digits/7B.png");
	SDL_Surface * G7 = IMG_Load("digits/7G.png");
	SDL_Surface * B8 = IMG_Load("digits/8B.png");
	SDL_Surface * G8 = IMG_Load("digits/8G.png");
	SDL_Surface * B9 = IMG_Load("digits/9B.png");
	SDL_Surface * G9 = IMG_Load("digits/9G.png");

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
			SDL_Rect dst = {x,y,size,size};
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
			SDL_Rect dst = {x,y,size,size};
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

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		return 1;
	}

	unsigned int len = 9*9;
	unsigned int * solved = calloc(len, sizeof (unsigned int));
	unsigned int * before = calloc(len, sizeof (unsigned int));

	file_to_array(argv[1], solved);
	file_to_array(argv[1], before);

	/*for(int i = 0; begin+i < end; i++)
	  {
	  if(i%9 != 8)
	  printf("%i ", *(array + i));
	  else
	  printf("%i\n", *(array + i));

	  }*/

	int err = solveSudo(solved, 0, 0, 9);
		
	if(err == 0)
		printf("fail");
	else
	{
		array_to_file(strcat(argv[1],".result"), solved);
		sudoku_to_img(before,solved);
	}

	free(solved);
	free(before);
	
	return 0;

}
