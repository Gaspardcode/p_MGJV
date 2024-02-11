#include "image.h"
#include "string.h"

//number of images in each directory : dir 0, dir 1, dir 2 ..
int cpts[10] = {0};
float cor = 3;
float cor_inv = (1./3.);
void scale(SDL_Surface ** img,float scale)
{

	int w = (*img)->w;
	int h = (*img)->h;
	w = (int)((float)w*scale);
	h = (int)((float)h*scale);
	printf("w:%d,h:%d, scale:%f\n",w,h,scale);
	SDL_Surface *up = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

	SDL_BlitScaled(*img,NULL,up,NULL);
	*img = up;
}
void save(SDL_Surface * img,int digit)
{
	//saves in the correct directory
	//finds out the new name to be given
	char path[22];
	snprintf(path, 22, "data/%d/%06d.png",digit,cpts[digit]++);
	//Uncomment below to see images in a decent size
	//scale(&img,6);
	IMG_SavePNG(img,path);
}
void move_right(SDL_Surface * img, int n)
{
	//moves the imaage n pixels to the right
	// the width should not be over 256 pixels
	int w = img->w;
	Uint32 *pixels = img->pixels;
	SDL_LockSurface(img);
	int tmp;
	for(unsigned char k = 0; k < n; k++)
	{
		for(unsigned char i = 0; i < w; i++)
		{
			tmp = pixels[(i+1)*w-1];
			for(unsigned char j = w-1; j > 1; j--)
				pixels[i*w+j] = pixels[i*w+j-1];
			pixels[i*w] = tmp;
		}
		
	}
	SDL_UnlockSurface(img);
}
void move_left(SDL_Surface * img, int n)
{
	//moves the imaage n pixels to the left
	// the width should not be over 256 pixels
	int w = img->w;
	Uint32 *pixels = img->pixels;
	SDL_LockSurface(img);
	int tmp;
	for(unsigned char k = 0; k < n; k++)
	{
		for(unsigned char i = 0; i < w; i++)
		{
			tmp = pixels[i*w];
			for(unsigned char j = 0; j < w-1; j++)
				pixels[i*w+j] = pixels[i*w+j+1];
			pixels[(i+1)*w-1] = tmp;
		}
	}
	SDL_UnlockSurface(img);
}
void move_up(SDL_Surface * img, int n)
{
	//moves the imaage n pixels to the left
	// the width should not be over 256 pixels
	int w = img->w;
	Uint32 *pixels = img->pixels;
	SDL_LockSurface(img);
	int tmp;
	for(unsigned char k = 0; k < n; k++)
	{
		for(unsigned char i = 0; i < w; i++)
		{
			tmp = pixels[i*w];
			for(unsigned char j = 0; j < w-1; j++)
				pixels[i+j*w] = pixels[i+(j+1)*w];
			pixels[(i+1)*w-1] = tmp;
		}
	}
	SDL_UnlockSurface(img);
}
void move_down(SDL_Surface * img, int n)
{
	//moves the imaage n pixels to the right
	// the width should not be over 256 pixels
	int w = img->w;
	Uint32 *pixels = img->pixels;
	SDL_LockSurface(img);
	int tmp;
	for(unsigned char k = 0; k < n; k++)
	{
		for(unsigned char i = 0; i < w; i++)
		{
			tmp = pixels[(i+1)*w-1];
			for(unsigned char j = w-1; j > 1; j--)
				pixels[i+j*w] = pixels[i+(j-1)*w];
			pixels[i*w] = tmp;
		}
		
	}
	SDL_UnlockSurface(img);
}
void gen_move(char * origin, int digit)
{
	SDL_Surface *img = load_image(origin);
	SDL_Surface *cpy = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);
	int gen = 5;
	int gen_ver = 3;

	//brand new image
	SDL_BlitSurface(img,NULL,cpy,NULL);
	for(unsigned char i = 1; i < gen;i++)
	{
		move_right(cpy,1);
		save(cpy,digit);
	}
	SDL_BlitSurface(img,NULL,cpy,NULL);
	for(unsigned char i = 1; i < gen;i++)
	{
		move_left(cpy,1);
		save(cpy,digit);
	}
	for(unsigned char i = 1; i < gen;i++)
	{
		SDL_BlitSurface(img,NULL,cpy,NULL);
		surface_to_rotate_shear(&cpy,(int)i*i*i);
		save(cpy,digit);
	}
	for(unsigned char i = 1; i < gen;i++)
	{
		SDL_BlitSurface(img,NULL,cpy,NULL);
		surface_to_rotate_shear(&cpy,(int)i*i*i*(-1));
		save(cpy,digit);
	}
	SDL_BlitSurface(img,NULL,cpy,NULL);
	for(unsigned char i = 1; i < gen_ver;i++)
	{
		move_up(cpy,(int)i);
		save(cpy,digit);
	}
	SDL_BlitSurface(img,NULL,cpy,NULL);
	for(unsigned char i = 1; i < gen_ver;i++)
	{
		move_down(cpy,(int)i);
		save(cpy,digit);
	}
}
void gen_dis(char * origin,int digit)
{
	//generates 4 new images from the first one
	//origin is the path to the start image
	SDL_Surface *img = load_image(origin);
	SDL_Surface *cpy = SDL_CreateRGBSurface(0,28,28,32,0,0,0,0);

	SDL_BlitSurface(img,NULL,cpy,NULL);
	surface_to_closing(cpy);
	save(cpy,digit);

	SDL_BlitSurface(img,NULL,cpy,NULL);
	scale(&cpy,cor);
	surface_to_opening(cpy);
	scale(&cpy,cor_inv);
	save(cpy,digit);

	SDL_BlitSurface(img,NULL,cpy,NULL);
	scale(&cpy,cor);
	surface_to_dilatation(cpy);
	scale(&cpy,cor_inv);
	save(cpy,digit);

	SDL_BlitSurface(img,NULL,cpy,NULL);
	scale(&cpy,cor);
	surface_to_erosion(cpy);
	scale(&cpy,cor_inv);
	save(cpy,digit);

	SDL_FreeSurface(cpy);
	SDL_FreeSurface(img);
}
void gen()
{
	char start[20] = {0};
	for(char i = 0; i < 10; i++)
	{
		snprintf(start,20,"data/%d/origin.png",i);
		gen_dis(start,i);
		gen_move(start,i);
	}
	char starta[21] = {0};
	for(char i = 0; i < 10; i++)
	{
		for(char j = 0; j < 12; j++)
		{
			snprintf(starta,21,"data/%d/%03dorigin.png",i,j);
			gen_dis(starta,i);
			gen_move(starta,i);
		}
	}
}
int main()
{
	gen();
	return 0;
}
