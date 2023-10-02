#include "../../../include/Image/struct.h"
#include "../../../include/Image/tools.h"

// Masks for computing the gradient magnitude
// Mask for AxeX
int sobel_Gx[9] = {1, 0, -1,
                   2, 0, -2,
                   1, 0, -1};

// Mask for AxeY
int sobel_Gy[9] = {1, 2, 1,
                   0, 0, 0,
                   -1, -2, -1};

unsigned char convolution(int* mask, Pix pixel)
{
	// Assuming pixel.r = pixel.g = pixel.b, ie. it's a grayscale pixel
	// Applies a convolution mask
	int convol = 0;
	
	for(int i = 0; i < MASK_LEN; i++)
	{
		Pix pix = pixel.neighb[i];
		convol += mask[i] * pix.b;
	}
	
	return (unsigned char)convol;
}

Image newImage(SDL_Surface * surface)
{
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	int * pixels = surface->pixels;
	Pix newpixels[len];
	SDL_PixelFormat* format = surface->format;	
	
	// surface is now useless
	SDL_FreeSurface(surface);

	// Fills the array a first time
	for(int i = 0; i < len; i++)
	{
		Pix pix;
		unsigned char r, g, b;
		SDL_GetRGB(pixels[i],format, &r, &g, &b);
		pix.color = pixels[i];
		pix.r = r;
		pix.g = g;
		pix.b = b;
		newpixels[i] = pix;
	}
	
	// Fills the neighbours array
	for(int i = 0; i < len; i++)
	{
		Pix neighb[9];
		int k = 0;
		for(int row = -1; row < 2 ; row++)
        	{
           		for(int bias = -1; bias < 2; bias++)
                	{
				//Formula for the neighbours index
                        	int j = i + row * w + bias;

                      		if(j >= 0 && j < len)
					//Pixel valid => Puts neighbour
					neighb[k] = newpixels[j];
                        	else
					//Pixel invalid => Puts current pixel
                                	neighb[k] = newpixels[i];
                        	
				//index in the array of neighbours
				k++;
                	}
        	}
		newpixels[i].neighb = neighb; 
	}
	
	Image img;
	img.w = w;
	img.h = h;
	img.pixels = newpixels;

	return img;
}

SDL_Surface* newSurface(Image *img)
{
	int w = img->w;
	int h = img->h;
	int len = w * h;
	int pixels[len];

	for(int i = 0; i < len; i++)
	{
		pixels[i] = img->pixels[i].color;
	}

	SDL_Surface* surf;
	surf = SDL_CreateRGBSurfaceFrom(pixels,w,h,0,0,0,0,0,0);
	
	return surf;
}
