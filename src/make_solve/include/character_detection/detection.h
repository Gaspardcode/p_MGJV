# ifndef DETECTION_H
# define DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../recognition/load_image.h"
#include "../recognition/ocr.h"
#include "../epurator/epurate.h"
#include "../separator/separate.h"
#include "../recognition/testfile.h"


int prepare(int* sudoku)
{
	int total = 0;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 1; j < 10; j++)
		{
			char file[80];
			sprintf(file, 
			"../make_solve/images/caracters/case%i%i.bmp", i, j);

			SDL_Surface *surface = IMG_Load(file);
			SDL_Surface *surf = 
				SDL_ConvertSurfaceFormat(surface, 
						SDL_PIXELFORMAT_RGB888, 0);

			Uint32* map = surf->pixels;
			int nb = 0;
			for(int u =0; u < 28; u++)
			{
				for(int v = 0; v < 28; v++)
				{
					Uint8 r, g, b;
					SDL_GetRGB(map[u*28 + v], 
					surf->format, &r,&g,&b);

					if(r < 128)
						nb++;
				}

			}
			total += nb;
			SDL_FreeSurface(surface);

		}
	}
	total = total/81;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 1; j < 10; j++)
		{
			char file[80];
			sprintf(file, 
			"../make_solve/images/caracters/case%i%i.bmp", i, j);
			SDL_Surface *surface = IMG_Load(file);
			SDL_Surface *surf = 
				SDL_ConvertSurfaceFormat(surface, 
						SDL_PIXELFORMAT_RGB888, 0);

			Uint32* map = surf->pixels;
			int nb = 0;
			for(int u =0; u < 28; u++)
			{
				for(int v = 0; v < 28; v++)
				{
					Uint8 r, g, b;
					SDL_GetRGB(map[u*28 + v], 
					surf->format, &r,&g,&b);

					if(r < 128)
						nb++;
				}

			}

			if(nb > total)
			{
				double* matrice = load_bits_image(file);
				double* outputs = predict(matrice);
				size_t max = 0;
				for(size_t resp = 0; resp < 10; resp++)
				{
					if(outputs[resp] > outputs[max])
					{
						max = resp;
					}
				}
				sudoku[i + ((j-1)*9)] = max;
			}
			else
			{
				sudoku[i + ((j-1)*9)] = 0;
			}
			SDL_FreeSurface(surface);

		}
	}
	return 1;




}
# endif
