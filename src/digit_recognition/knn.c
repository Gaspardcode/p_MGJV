#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image.h"
#define DATA_S 2640
#define IMG_S (28*28) 
typedef struct img_d
{
	int grp;
	int data[IMG_S];
}img_d;

float * dists;
img_d * data[DATA_S];
//SDL_Surface * test[DATA_S];

img_d * img_digit(char * filename)
{
	//convert the image located at filename intothe struct img_d 
	SDL_Surface * img = load_image(filename);
	surface_to_invert(img);
	//Struct declaration
	img_d * image = malloc(sizeof(img_d));

	int * pixels = img->pixels;
	int * bits = image->data;
	SDL_LockSurface(img);

	for(int i = 0; i < IMG_S; i++)
	       bits[i] = (pixels[i] > 0) ? 1 : 0;

	SDL_UnlockSurface(img);
	return image;
}
void start()
{
	char path[18];
	int cpt = -1;

	dists = malloc(DATA_S*sizeof(float));

	for(int i = 0; i < DATA_S;i++)
	{
		dists[i] = 32000;

		if(i % 264 == 0)
			cpt++;

		snprintf(path,18,"data/%d/%06d.png",cpt,i%264);
		//printf("%s\n",path);

		data[i] = img_digit(path);
		data[i]->grp = cpt;
	}
}
void distances(int * new_digit)
{
	for(int i = 0; i < DATA_S; i++)
	{
		if(data[i]->grp < 10)
		{
			float sum = 0;
			for(int j = 0; j < IMG_S; j++)
			{
				int v = ((data[i]->data)[j]-new_digit[j]); 
				sum += v*v;
			}	
			sum = (float)sqrt((double)sum);
			dists[i] = sum;
			printf("Distance from %d : %f\n",data[i]->grp,sum);
		}
	}
}
int min_flt(float * array)
{
        float min = 32000;
        int index = 0;
        for(int i = 0; i < DATA_S; i++)
        {
                index = array[i] < min ? i : index;
                min = array[i] < min ? array[i] : min;
        }
        return index;
}
int predict(char * digits)
{
	//finds the nearest group
	char max = 0;
        int index = 0;
        for(int i = 0; i < 10; i++)
        {
                index = digits[i] > max ? i : index;
                max = digits[i] > max ? digits[i] : max;
        }
        return index;
}
void group(char * filename, int k)
{
	img_d * new_digit = img_digit(filename);
	// Finds out the distances to all number in the dataset
	char digits[10] = {0};
	distances(new_digit->data);
	// Finds the nearest N 
	for(int i = 0; i < k; i++)
	{
		int min = min_flt(dists);
		digits[data[min]->grp]++;
		dists[min] = 32000;
	}

	//int prediction = predict(digits);
	printf("Prediction : ");
	for(int i = 0; i < 10; i++)
		printf(" %d : %1f%% |",i,((float)digits[i]*100./(float)k));
	printf("\n");
}
int main()
{
	start();
	int k = 10;
	printf("Expected : %d\n",8);
	group("test.png",k);
	printf("Expected : %d\n",6);
	group("0test.png",k);
	printf("Expected : %d\n",0);
	group("1test.png",k);
	printf("Expected : %d\n",9);
	group("2test.png",k);
	return 0;
}
