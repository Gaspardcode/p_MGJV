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
#include "load_image.h"

#define INPUT 784
#define HIDDEN 30
#define OUTPUT 10


//definition of weight and bias array
double** h_weight;
double* h_bias;
double** o_weight;
double* o_bias;

double softmax_derivative(double softmax_output) 
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

double* predict(double* inputs)
{
  h_weight = malloc(HIDDEN * sizeof(double*));
  for(size_t i = 0; i<HIDDEN; i++)
    h_weight[i] = calloc(INPUT,sizeof(double));

  o_weight = malloc(OUTPUT * sizeof(double*));
  for(size_t i = 0; i<OUTPUT; i++)
    o_weight[i] = calloc(HIDDEN,sizeof(double));

  h_bias = calloc(HIDDEN, sizeof(double));
  o_bias = calloc(OUTPUT, sizeof(double));

  insert_list(INPUT, 
	"../make_solve/include/recognition/hweight2.txt", h_weight);
  insert_list(HIDDEN, 
	"../make_solve/include/recognition/oweight2.txt", o_weight);
  insert_list(OUTPUT, 
	"../make_solve/include/recognition/obias2.txt", &o_bias);
  insert_list(HIDDEN, 
	"../make_solve/include/recognition/hbias2.txt", &h_bias);


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
    //output = sigmoid(output + o_bias[i]);
    outputs[i] = output;
  }
  outputs = softmax(outputs);

  free(h_weight);
  free(o_weight);
  free(h_bias);
  free(o_bias);

  return outputs;
}

#endif
