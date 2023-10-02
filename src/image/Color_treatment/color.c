#include "../../../include/Image/generic.h"
void image_to_grayscale(Image* img)
{
	int w = img->w;
	int h = img->h;
	for(int i = 0; i < w * h; i++)
	{
		Pix pix = img->pixels[i]; 	
		float avg = 0.3*pix.r + 0.59*pix.g + 0.11*pix.b;
		img->pixels[i].r = img->pixels[i].g = img->pixels[i].b = avg;
	}
}
