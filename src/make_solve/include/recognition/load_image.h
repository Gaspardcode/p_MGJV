# ifndef _LOAD_IMAGE_H
# define _LOAD_IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void printer(double* image)
{

  printf("\n\n");
  for(size_t i = 0; i < 28*28; i++)
  {
    if(i%28 == 0)
      printf("\n");

    printf("%.0f", image[i]);

  }
  printf("\n\n");
}


// Assuming your surface has 1 byte per pixel (indexed color)
Uint8 getPixelColor(SDL_Surface* surface, int x, int y) {
  if (surface == NULL || x < 0 || y < 0 || x >= surface->w || y >= surface->h) {
    // Invalid input
    return 0;
  }

  // Calculate the offset to the pixel
  int offset = y * surface->pitch + x * surface->format->BytesPerPixel;

  // Get the pixel value
  Uint8 pixelValue = *((Uint8*)(surface->pixels) + offset);

  return pixelValue;
}

double* load_image_revert(int argc, char **argv)
{
  if(argc < 1)
    printf("The function need an argument");
  char* file = argv[1];
  //printf("%s is loading\n", argv[1]);
  SDL_Surface *surface = IMG_Load(file);
  if(surface->format->BytesPerPixel == 1)
  {
    //Uint8 *surf_map = surface->pixels;
    double* map8 = calloc(28*28, sizeof(double));

    for(size_t i = 0; i < 28; i++)
    {
      for(size_t j = 0; j < 28; j++)
      {
        Uint8 b =getPixelColor(surface, i, j);

        if(b < 128) 
        {
          map8[i*28 + j] = 1;
        }
      }
    }
    double* map4 = calloc(28*28, sizeof(double));
    for(size_t x = 0; x < 28; x++)
    {
      for(size_t y = 0; y < 28; y++)
      {
        map4[x*28 + y] = map8[x + y*28];
      }
    }
    printer(map4);
    return map4;


  }
  SDL_Surface *surf = 
	SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
  Uint32 *surf_map = surf->pixels;
  double* map = calloc(28*28, sizeof(double));
  for(size_t i = 0; i < 28*28; i++)
  {
    Uint8 r, g, b;
    SDL_GetRGB(surf_map[i], surface->format, &r,&g,&b);
    if(r < 10 && g < 10 && b < 10) 
    {
      map[i] = 1;
    }

  }
  double* map2 = calloc(28*28, sizeof(double));
  for(size_t x = 0; x < 28; x++)
  {
    for(size_t y = 0; y < 28; y++)
    {
      map2[x*28 + y] = map[x + y*28];
    }
  }

  printer(map2);
  return map2;
}
//load image for MNIST
double* load_bits_image(char *argv)
{
  char* file = argv;
  //printf("%s is loading\n", argv[1]);
  SDL_Surface *surface = SDL_LoadBMP(file);
  //printf("%i\n", surface->format->BytesPerPixel);
  if(surface->format->BytesPerPixel == 1)
  {
    //Uint8 *surf_map = surface->pixels;
    double* map8 = calloc(28*28, sizeof(double));

    for(size_t i = 0; i < 28; i++)
    {
      for(size_t j = 0; j < 28; j++)
      {
        Uint8 b =getPixelColor(surface, i, j);

        if(b < 128) 
        {
          map8[i*28 + j] = 1;
        }
      }
    }
    //printer(map8);
    return map8;


  }
  SDL_Surface *surf = 
	SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
  //printf("%i", surface->format->BytesPerPixel);

  Uint32 *surf_map = surf->pixels;
  //SDL_Surface *surf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
  //Uint32 *surf_map = surf->pixels;
  double* map = calloc(28*28, sizeof(double));

  for(size_t i = 0; i < 28*28; i++)
  {
    Uint8 r, g, b;
    SDL_GetRGB(surf_map[i], surface->format, &r,&g,&b);
    if(r < 216 && g < 32 && b < 32) 
    {
      map[i] = 1;
    }

  }
  //printer(map);
  return map;
}
# endif
