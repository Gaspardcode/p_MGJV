# ifndef EPURATOR_H
# define EPURATOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void rec_draw(SDL_Surface *surface, int x, int y, 
		int* bool, SDL_Surface *new_surface)
{

	if(bool[x + y*28] == 1)
		return;

	Uint32 *surf_map = surface->pixels;
	Uint8 r, g, b;
	SDL_GetRGB(surf_map[x + y*28], surface->format, &r,&g,&b);

	Uint32 *pix = new_surface->pixels;

	if(r > 128 && g > 16 && b > 16) 
	{
		bool[x + y*28] = 1;
		pix[x + y*28] = SDL_MapRGB(new_surface->format, 255, 255, 255);
		return;
	}
	else
	{
		bool[x + y*28] = 1;
		pix[x + y*28] = SDL_MapRGB(surface ->format, 0, 0, 0);

		if(y > 0)
		{
			rec_draw(surface, x, y-1, bool, new_surface);
		}
		if(x > 0)
		{
			rec_draw(surface, x-1, y, bool, new_surface);
		}

		if(y < 27)
		{
			rec_draw(surface, x, y+1, bool, new_surface);
		}
		if(x < 27)
		{
			rec_draw(surface, x+1, y, bool, new_surface);
		}
	}

}


SDL_Surface* drawer(SDL_Surface *surface, int x, int y)
{
	SDL_Surface *imageN = NULL;
	imageN = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
	SDL_Surface *image = 
		SDL_ConvertSurfaceFormat(imageN, SDL_PIXELFORMAT_RGB888, 0);
	SDL_LockSurface(image);
	Uint32* pix = image->pixels;
	int* bool = calloc(28*28, sizeof(int));
	for (int i = 0; i < 28*28; i++)
	{
		bool[i] = 0;
		pix[i] = SDL_MapRGB(image->format, 255,255,255);
	}
	rec_draw(surface, x, y, bool, image);
	SDL_UnlockSurface(image);

	//SDL_SaveBMP(image, "finalepu.bmp");
	return image;
}

void rec_count(SDL_Surface *surface, int x, int y, int* bool, int* sum)
{

	if(bool[x + y*28] == 1)
		return;

	Uint32 *surf_map = surface->pixels;
	Uint8 r, g, b;
	SDL_GetRGB(surf_map[x + y*28], surface->format, &r,&g,&b);

	if(r > 228 && g > 228 && b > 228) 
	{
		bool[x + y*28] = 1;
		surf_map[x + y*28] = SDL_MapRGB(surface ->format, 217, 33, 33);

		return;
	}
	else
	{
		surf_map[x + y*28] = SDL_MapRGB(surface ->format, 0, 0, 0);
		bool[x + y*28] = 1;
		*sum += 1;
		if(y > 0)
		{
			rec_count(surface, x, y-1, bool, sum);
		}
		if(x > 0)
		{
			rec_count(surface, x-1, y, bool, sum);
		}

		if(y < 27)
		{
			rec_count(surface, x, y+1, bool, sum);
		}
		if(x < 27)
		{
			rec_count(surface, x+1, y, bool, sum);
		}
	}

}

SDL_Surface* search(SDL_Surface *surface)
{
	int* bool = calloc(28*28, sizeof(int));
	int* sum = calloc(1, sizeof(int));
	Uint32 *surf_map = surface->pixels;

	*sum = 0;
	int temp_sum = 0;
	int X = 0;
	int Y = 0;
	SDL_LockSurface(surface);


	for(int i = 0; i < 28; i++)
	{
		for(int j = 0; j < 28; j++)
		{
			bool[i + j*28] = 0;
		}
	}
	for(int i = 0; i < 28; i++)
	{
		for(int j = 0; j < 28; j++)
		{
			if(bool[i + j*28] == 0)
			{
				rec_count(surface, i, j, bool, sum);
				if(*sum > temp_sum)
				{
					temp_sum = *sum;
					X = i;
					Y = j;

					surf_map[X + Y*28] = 
						SDL_MapRGB(surface ->format, 
								33, 128, 33);

				}
				*(sum) = 0;
			}
		}
	}
	surf_map[X + Y*28] = SDL_MapRGB(surface ->format, 33, 33, 133);

	SDL_UnlockSurface(surface);
	//SDL_SaveBMP(surface, "epu.bmp");
	SDL_Surface* clean_case = drawer(surface, X, Y);
	return clean_case;
}






void propagation(SDL_Surface *surface)
{
	int w = surface->w;
	int h = surface->h;
	Uint32 *surf_map = surface->pixels;
	SDL_LockSurface(surface);
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < w; j++)
		{
			surf_map[j + i*w] = SDL_MapRGB(surface->format, 
					255, 255, 255);	
		}
	}
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			surf_map[j + i*h] = SDL_MapRGB(surface->format, 
					255, 255, 255);	
		}
	}
	for(int i = h-4; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			surf_map[j + i*w] = SDL_MapRGB(surface->format, 
					255, 255, 255);	
		}
	}
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			surf_map[w*i + (h-j)] = SDL_MapRGB(surface->format, 
					255, 255, 255);	
		}
	}
	SDL_UnlockSurface(surface);

	//SDL_SaveBMP(surface, "out.bmp");

}


int epure(int black_on_white)
{

	for(int i = 0; i < 9; i++)
	{
		for(int j = 1; j < 10; j++)
		{
			char file[] = "../make_solve/images/cases/case__.bmp";
			file[31] = i + '0';
			file[32] = j + '0';
			char output_file[] = 
			"../make_solve/images/caracters/case__.bmp";
			output_file[35] = i+'0';
			output_file[36] = j+'0';
			SDL_Surface *surface = IMG_Load(file);
			SDL_Surface *surf = 
				SDL_ConvertSurfaceFormat(surface, 
						SDL_PIXELFORMAT_RGB888, 0);
      //if the image is white on black (Most of the time)
			if(black_on_white == 0)
			{
				Uint32 *surf_map = surf->pixels;
				for(size_t v = 0; v < 784; v++)
				{
					Uint8 r, g, b;
					SDL_GetRGB(surf_map[v], 
					surface->format, &r,&g,&b);

					if(r < 128)
					{
						surf_map[v] =
						SDL_MapRGB(surf->format, 
						255, 255, 255);
					}
					else
					{
						surf_map[v] =
						SDL_MapRGB(surf->format, 
						128, 128, 128);
					}


				}
			}
			propagation(surf);
			SDL_Surface* clean_case = search(surf);
			SDL_SaveBMP(clean_case, output_file);

			SDL_FreeSurface(surface);

		}
	}
	return 0;

}

# endif
