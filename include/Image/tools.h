#pragma once
#include <math.h>
#define MASK_LEN 9
#define MAGN(Gx,Gy) sqrt(Gx*Gx + Gy*Gy) 
unsigned char convolution(int* mask, Pix pixel);
