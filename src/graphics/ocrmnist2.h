#ifndef OCR_MNIST_H
#define OCR_MNIST_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <err.h>
#include <locale.h>
//#include "mnist.h"

//#include "load_image.h"

#define INPUT 784
#define HIDDEN 30
#define OUTPUT 10

//definition of weight and bias array
double** h_weight2;
double* h_bias2;
double** o_weight2;
double* o_bias2;


void insert_list2(size_t a, size_t b, char* file, double** tab) // recupere les donnees de file et les mets dans tab
{
    setlocale(LC_NUMERIC, "C");
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        err(1,"open()");

    int r;
    char buffer[64];
    char nbr[80];
    size_t k =0;
    size_t z =0;
    size_t i = 0;
    a++;
    while((r = read(fd, buffer, 64)) != 0)
    {
        int j = 0;
        while(j < r)
        {
            char* eptr;
            double result;
            if(buffer[j] == '\n')
            {
                nbr[i] = 0;
                result = strtod(nbr, &eptr);
                if (*eptr != '\0')
                { 
                    fprintf(stderr, "Erreur de conversion à la position %zu\n", eptr -nbr);
                }
                i =0;
                j++;
                tab[k][z] = result;
                z++;
                if(z == b)
                {
                    z = 0;
                    k++;
                }
            }
            else
            {
                nbr[i] = buffer[j];
                i++;
                j++;
            }
        }
    }
    close(fd);

}



void replace2(size_t a, size_t b, char* file, double** tab) // detruit le fichier file et le remplace par un nouveau avec les valeurs de tab
{
    if(remove(file) ==-1)
        err(1,"remove()");
    char str[80];
    int fd = open(file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        err(1, "open()");
    for( size_t i =0; i<a;i++)
    {
        for(size_t j =0; j <b;j++)
        {

            sprintf(str, "%f\n", tab[i][j]);
            if(write(fd, str, strlen(str)) == -1)
                err(1, "write()");

        }
    }
    close(fd);
}


void libere()// libere les espaces aloué
{
    replace2(HIDDEN,INPUT,"hweight2.txt",h_weight2);
    replace2(OUTPUT,HIDDEN, "oweight2.txt",o_weight2);
    replace2(1,HIDDEN,"hbias2.txt", &h_bias2);
    replace2(1,OUTPUT,"obias2.txt", &o_bias2);


    for(size_t i = 0; i< HIDDEN;i++)
        free(h_weight2[i]);
    free(h_weight2);
    for(size_t i = 0; i< OUTPUT;i++)
        free(o_weight2[i]);
    free(o_weight2);
    free(h_bias2);
    free(o_bias2);
}

















double softmax_derivative2(double softmax_output)
{
    return softmax_output * (1.0 - softmax_output);
}


double* softmax2(double* list)
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





void chargement2()
{
    h_weight2 = malloc(HIDDEN * sizeof(double*));
    for(size_t i = 0; i<HIDDEN; i++)
        h_weight2[i] = calloc(INPUT,sizeof(double));

    o_weight2 = malloc(OUTPUT * sizeof(double*));
    for(size_t i = 0; i<OUTPUT; i++)
        o_weight2[i] = calloc(HIDDEN,sizeof(double));

    h_bias2 = calloc(HIDDEN, sizeof(double));
    o_bias2 = calloc(OUTPUT, sizeof(double));

    insert_list2(HIDDEN, INPUT, "hweight2.txt", h_weight2);
    insert_list2(OUTPUT, HIDDEN, "oweight2.txt", o_weight2);
    insert_list2(1,OUTPUT, "obias2.txt", &o_bias2);
    insert_list2(1,HIDDEN, "hbias2.txt", &h_bias2);

}



void learn2(double inputs[INPUT], double targets[OUTPUT], double alpha)
{
    chargement2();
    // Forward pass through the hidden layer
    double hiddens[HIDDEN];
    for (size_t i = 0; i < HIDDEN; i++)
    {
        double hidden = 0;
        for (size_t j = 0; j < INPUT; j++)
        {
            // Weighted sum of inputs passed through the sigmoid activation function
            hidden += h_weight2[i][j] * inputs[j];
        }
        hidden = sigmoid(hidden + h_bias2[i]);
        hiddens[i] = hidden;
    }

    // Forward pass through the output layer
    double* outputs = calloc(10, sizeof(double));
    for (size_t i = 0; i < OUTPUT; i++)
    {
        double output = 0;
        for (size_t j = 0; j < HIDDEN; j++)
        {
            // Weighted sum of hidden layer outputs passed through the softmax activation function
            output += o_weight2[i][j] * hiddens[j];
        }
        outputs[i] = output;
    }
    outputs = softmax2(outputs);

    // Compute errors between target outputs and actual outputs
    double errors[OUTPUT];
    for (size_t i = 0; i < OUTPUT; i++)
    {
        double error = targets[i] - outputs[i];
        errors[i] = error;
    }

    // Compute derivative of errors with respect to output layer outputs
    double derrors[OUTPUT];
    for (size_t i = 0; i < OUTPUT; i++)
    {
        double derror = errors[i] * softmax_derivative2(outputs[i]);
        derrors[i] = derror;
    }

    // Backward pass: Compute the contribution of each hidden layer neuron to the error
    double ds[HIDDEN];
    for (size_t i = 0; i < HIDDEN; i++)
    {
        ds[i] = 0;
    }
    for (size_t i = 0; i < OUTPUT; i++)
    {
        for (size_t j = 0; j < HIDDEN; j++)
        {
            // Accumulate contributions to the error from the output layer
            ds[j] += derrors[i] * o_weight2[i][j] * sigmoid_prime(hiddens[j]);
        }
    }

    // Update weights and biases in the output layer
    for (size_t i = 0; i < OUTPUT; i++)
    {
        for (size_t j = 0; j < HIDDEN; j++)
        {
            o_weight2[i][j] += alpha * hiddens[j] * derrors[i];
        }
        o_bias2[i] += alpha * derrors[i];
    }
    free(outputs);

    // Update weights and biases in the hidden layer
    for (size_t i = 0; i < HIDDEN; i++)
    {
        for (size_t j = 0; j < INPUT; j++)
        {
            h_weight2[i][j] += alpha * inputs[j] * ds[i];
        }
        h_bias2[i] += alpha * ds[i];
    }
    libere();
}


double* predict2(double* inputs)
{

    /*h_weight = malloc(HIDDEN * sizeof(double*));
      for(size_t i = 0; i<HIDDEN; i++)
      h_weight[i] = calloc(INPUT,sizeof(double));

      o_weight = malloc(OUTPUT * sizeof(double*));
      for(size_t i = 0; i<OUTPUT; i++)
      o_weight[i] = calloc(HIDDEN,sizeof(double));

      h_bias = calloc(HIDDEN, sizeof(double));
      o_bias = calloc(OUTPUT, sizeof(double));

      insert_list(HIDDEN, INPUT, "../neural_net/hweight2.txt", h_weight);
      insert_list(OUTPUT, HIDDEN, "../neural_net/oweight2.txt", o_weight);
      insert_list(1,OUTPUT, "../neural_net/obias2.txt", &o_bias);
      insert_list(1,HIDDEN, "../neural_net/hbias2.txt", &h_bias);*/


    double hiddens[HIDDEN];
    for(size_t i =0; i<HIDDEN;i++)
    {
        double hidden = 0;
        for(size_t j = 0; j<INPUT;j++)
            hidden+= h_weight2[i][j] * inputs[j];
        hidden = sigmoid(hidden +h_bias2[i]);
        hiddens[i] = hidden;
    }

    double* outputs = calloc(OUTPUT, sizeof(double));
    double output = 0;
    for(size_t i =0; i<OUTPUT;i++)
    {
        output =0;
        for(size_t j =0; j < HIDDEN; j++)
            output+= o_weight2[i][j] * hiddens[j];
        //output = sigmoid(output + o_bias[i]);
        outputs[i] = output;
    }
    outputs = softmax2(outputs);
    return outputs;
}






#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void printer2(double* image)
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
Uint8 getPixelColor2(SDL_Surface* surface, int x, int y) 
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
double* load_image2(int argc, char *argv)
{
    if(argc < 1)
        printf("The function need an argument");
    char* file = argv;
    printf("%s is loading\n", argv);
    SDL_Surface *surface = IMG_Load(file);
    SDL_Surface *surf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    Uint32 *surf_map = surf->pixels;
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
    double* map2 = calloc(28*28, sizeof(double));
    for(size_t x = 0; x < 28; x++)
    {
        for(size_t y = 0; y < 28; y++)
        {
            map2[x*28 + y] = map[x + y*28];
        }
    }

    printer2(map2);
    return map2;
}
/*
double* load_image2(int argc, char **argv)
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
        if(r < 216 && g < 32 && b < 32) 
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
}*/


int prediction2(int argc, char* arg)
{
    if (argc == 0)
        errx(1, "Rajouter un argument");

    chargement2();
    double* inputs = load_image2(1,arg);
    double* result = predict2(inputs);
    free(inputs);
    int max = 0;
    for(int i = 0; i<10;i++)
    {
        printf("Number %i: %f\n",i, result[i]);
        if(result[max] < result[i])
            max = i;
    }
    printf("🔮 Prédiction: %i\n", max);
    libere();
    return max;
}

#endif
