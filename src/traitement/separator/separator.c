#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void cut_and_save(SDL_Surface* surface, int x, int y, int nb1, int nb2) 
{
	int w = surface->w;
	//int h = surface->h;
	Uint32 *surf_map = surface->pixels;
	SDL_Surface *imageN = NULL;
	int nw= 256/9; 
	int nh = 256/9;
	imageN = SDL_CreateRGBSurface(0, nw, nh, 32, 0, 0, 0, 0);
	SDL_Surface *image = 
		SDL_ConvertSurfaceFormat(imageN, SDL_PIXELFORMAT_RGB888, 0);
	SDL_LockSurface(image);
	SDL_LockSurface(surface);
	Uint32* surf_image = image->pixels;
	//x and y are not good there are dimensions problematics

	for (int j = 0; j < nh; j++)
	{
		for(int i = 0; i < nw; i++)
		{
			Uint8 r, g, b;
			SDL_GetRGB(surf_map[(i+x) + ((j+y)*w)], 
					surface->format, &r,&g,&b);
			surf_image[i + j*nw] = 
				SDL_MapRGB(image->format, r, g, b);
		}
	}
	SDL_UnlockSurface(image);
	SDL_UnlockSurface(surface);
	char name[] = "../images/cases/case__.bmp";
	name[21] = (nb2%10)+'0';
	name[20] = (nb1) + '0';
	SDL_SaveBMP(image, name);
	SDL_FreeSurface(image);
}

int main(int argc, char** argv)
{
	if(argc != 2)
		errx(EXIT_FAILURE, "Usage: image file");
	SDL_Surface *surface = IMG_Load(argv[1]);
	SDL_Surface *surf = 
		SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
	for(int i = 0; i < 9; i++)
	{
		for(int j = 1; j < 10; j++)
		{
			cut_and_save(surf, i*28, (j-1)*28, i, j);
		}
	}
	SDL_FreeSurface(surface);
	return 0;
}
