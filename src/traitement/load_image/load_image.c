#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

/* to use this load function you must call it with a character 
 * image 28 pixels by 28 (../image/caracters/case__.bmp)
 * the printer function is a test function to see the result of
 * the printed matrice */

void printer(int* image)
{
	for(size_t i = 0; i < 28*28; i++)
	{
		if(i%28 == 0)
			printf("\n");

		printf("%i", image[i]);

	}
}

int main(int argc, char **argv)
{
	if(argc < 1)
		printf("The function must be run with an argument");
	char* file = argv[1];
	printf("%s is loading\n", argv[1]);
	SDL_Surface *surface = IMG_Load(file);
	SDL_Surface *surf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
	Uint32 *surf_map = surf->pixels;
	int* map = calloc(28*28, sizeof(int));
	for(size_t i = 0; i < 28*28; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(surf_map[i], surface->format, &r,&g,&b);
		if(r < 216 && g < 32 && b < 32) 
		{
			map[i] = 1;
		}

	}
	printer(map);
	return 0;
}	
