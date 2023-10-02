#ifndef S_H
#define S_H

#include <stdlib.h> /* malloc, realloc, free, NULL */
#include <stdio.h> /* fprintf, stderr */
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

typedef struct Pix
{
	unsigned char r, g, b;
	Uint32 color;
	struct Pix* neighb;
} Pix;

typedef struct Image
{
	unsigned int w;
	unsigned int h;
	Pix* pixels;
} Image;



 Image newImage(SDL_Surface* surface);
 /*
 *
 *
 *
 */
 SDL_Surface* newSurface(Image *image);
 
 SDL_Surface * load_image(const char* path);
#endif
