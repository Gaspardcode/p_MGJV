#ifndef OCR_H
#define OCR_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
//#include "mnist.h"
#include "testfile.h"
//#include "load_image.h"

#define INPUT 784
#define HIDDEN 30
#define OUTPUT 10

//definition of weight and bias array
double** h_weight;
double* h_bias;
double** o_weight;
double* o_bias;

double softmax_derivative(double softmax_output, double target_output)
{
    return softmax_output * (1.0 - softmax_output);
}
double soft_max(double* outputs, double x)
{
    double sum = 0;
    for(size_t i = 0; i < 10; i++)
    {
        sum += exp(outputs[i]);
    }
    return (exp(x)/sum);
}

double* softmax(double* list)
{
    double* res = calloc(10, sizeof(double));
    double somme = 0;
    for(size_t i =0; i <10; i++)
    {
        somme+= exp(list[i]);
    }
    for(size_t j = 0; j<10; j++)
    {
        res[j] = exp(list[j])/somme;
    }
    return res;
}


double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_prime(double x)
{
    return x * (1-x);
}


void chargement()
{
    h_weight = malloc(HIDDEN * sizeof(double*));
    for(size_t i = 0; i<HIDDEN; i++)
        h_weight[i] = calloc(INPUT,sizeof(double));

    o_weight = malloc(OUTPUT * sizeof(double*));
    for(size_t i = 0; i<OUTPUT; i++)
        o_weight[i] = calloc(HIDDEN,sizeof(double));

    h_bias = calloc(HIDDEN, sizeof(double));
    o_bias = calloc(OUTPUT, sizeof(double));

    insert_list(HIDDEN, INPUT, "hweight.txt", h_weight);
    insert_list(OUTPUT, HIDDEN, "oweight.txt", o_weight);
    insert_list(1,OUTPUT, "obias.txt", &o_bias);
    insert_list(1,HIDDEN, "hbias.txt", &h_bias);

}

double* predict(double* inputs)
{

    double hiddens[HIDDEN];
    for(size_t i =0; i<HIDDEN;i++)
    {
        double hidden = 0;
        for(size_t j = 0; j<INPUT;j++)
            hidden+= h_weight[i][j] * inputs[j];
        hidden = sigmoid(hidden +h_bias[i]);
        hiddens[i] = hidden;
    }

    double* outputs = calloc(OUTPUT, sizeof(double));
    double output = 0;
    for(size_t i =0; i<OUTPUT;i++)
    {
        output =0;
        for(size_t j =0; j < HIDDEN; j++)
            output+= o_weight[i][j] * hiddens[j];
        outputs[i] = output;
    }
    outputs = softmax(outputs);
    return outputs;
}

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
Uint8 getPixelColor(SDL_Surface* surface, int x, int y) 
{
    if (surface == NULL || x < 0 || y < 0 || x >= surface->w || y >= surface->h) 
    {
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
    printf("%s is loading\n", argv[1]);
    SDL_Surface *surface = IMG_Load(file);
    Uint8 *surf_map = surface->pixels;
    double* map8 = calloc(28*28, sizeof(double));

    for(size_t i = 0; i < 28; i++)
    {
        for(size_t j = 0; j < 28; j++)
        {
            Uint8 b = getPixelColor(surface, i, j);
            //printf("%i\n",b);

            if(b < 128)
            {
                map8[i*28 + j] = 1;
            }
        }
    }
    double *map4 = calloc(28*28, sizeof(double));
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

double* load_image(int argc, char **argv)
{
    if(argc < 1)
        printf("The function need an argument");
    char* file = argv[1];
    printf("%s is loading\n", argv[1]);
    SDL_Surface *surface = IMG_Load(file);
    SDL_Surface *surf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    Uint32 *surf_map = surf->pixels;
    double* map = calloc(28*28, sizeof(double));
    for(size_t i = 0; i < 28*28; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(surf_map[i], surface->format, &r,&g,&b);
        printf("%i %i %i\n",r,g,b);
        if(r < 216 && g < 32 && b < 32) 
        {
            map[i] = 1;
        }

    }
    printer(map);
    return map;
}

#endif
