#pragma once
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#define INIT_HEIGHT 1000
#define INIT_WIDTH 1000
#define PIVOT 400 // MAX BEING 255 + 255 + 255 = 765 GOOD BEING 350 
#define WHITE 255
#define BLACK 0
#define MAX_PIX_VAL 256
#define NB_FILTER 1
#define BNW 0
#define MASK_LEN_SOBEL 9
#define MASK_LEN_GAUSS 25 
#define PI 3.14159265
void surface_to_resize(SDL_Surface * surface, const double scale)
void surface_to_resize_border(SDL_Surface ** surface, const int nw, const int nh)
void surface_to_rotate_shear(SDL_Surface * surface, const int theta)
void surface_to_rotate(SDL_Surface * surface,const int theta)
int max_arr(int* array, int len)
void Draw_Polar_Line(SDL_Surface * surface, double r, double ct, double st)
void Draw_Line(SDL_Surface * surface, double m, double b)
void delete_k_neigh(int * arr, int k, int rho, int theta ,int w, int h)
void surface_to_hough(SDL_Surface * surface, int * min_theta)
void RandomLines(SDL_Surface * surface, int n)
void surface_to_adaptive_treshold(SDL_Surface * surface, int size)
void surface_to_erosion(SDL_Surface * surface)
void surface_to_dilatation(SDL_Surface * surface)
void surface_to_opening(SDL_Surface * surface)
void surface_to_closing(SDL_Surface * surface)
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* treatment)
SDL_Surface* load_image(const char* path)
// Masks for computing the gradient magnitude
// Mask for AxeX
int sobel_Gx[9] = {-1, 0, 1,
                   -2, 0, 2,
                   -1, 0, 1};

// Mask for AxeY
int sobel_Gy[9] = {-1, -2, -1,
                   0, 0, 0,
                   1, 2, 1};

// Mask for experimentation
int sobel_Gz[9] = {0, 1, 2,
                   -1, 0, 1,
                   -2, -1, 0};

// Mask for experimentation
int prewitt_Gx[9] = {-1, 0, 1,
                   -1, 0, 1,
                   -1, 0, 1};

// Mask for experimentation
int prewitt_Gy[9] = {-1, -1, -1,
                      0, 0, 0,
                      1, 1, 1};
// Mask for experimentation
int kirch_Gx[9] = {   -3, -3, 5,
                      -3,  0, 5,
                      -3, -3, 5};

// Mask for experimentation
int kirch_Gy[9] = {   -3, -3, -3,
                      -3,  0, -3,
                       5,  5,  5};

// Gaussian blur mask 5x5
int Gaussian[25] = {   2,   4,   5,  4, 2,
                      4,   9,  12,  9, 4,
                      5,  12,  15, 12, 5,
                      4,   9,  12,  9, 4,
		      2,   4,   5,  4, 2    };
// Gaussian blur mask 5x5
int Gaussian_sigma_one[25] = {   1,    4,   7,   4, 1,
                      		 4,   16,  26,  16, 4,
                      		 7,   26,  41,  26, 7,
                      		 4,   16,  26,  16, 4,
		      		 1,    4,   7,   4, 1    };


double convolution(Uint8* pixels, int *mask, int len)
double pixel_to_blur(Uint8* pixels)
int pixel_to_angle(Uint8* pixels)
double sobel_magnitude(Uint8* pixels)
void surface_to_blur(SDL_Surface * surface)
void surface_to_canny(SDL_Surface * surface)
int min(Uint8* array, int start)
int min_n(Uint8* array, int len)
int max_n(Uint8* array, int len)
Uint32 pixel_to_normalize(Uint32 pixel_color, SDL_PixelFormat* format, int min_r, int max_r,int min_g,int max_g,int min_b,int max_b)
void surface_to_normalize(SDL_Surface* surface)
void sort(Uint8* medians)
Uint32 pixel_to_median(Uint32* pixels,Uint32 pixel_color, SDL_PixelFormat* format, int i, int len, int w)
void surface_to_median(SDL_Surface* surface)
Uint32 pixel_to_invert(Uint32 pixel_color, SDL_PixelFormat* format)
void surface_to_invert(SDL_Surface* surface)
long double w1_t(int t, int n, int * histogram)
long double w2_t(int t, int n, int * histogram)
long double P_i(int i, int n, int * histogram)
long double u1_t(int t, int n, int * histogram)
double u2_t(int t, int n, int * histogram)
long double sigma_b_t(int t, int n, int * histogram)
int Otsu(SDL_Surface * surface)
int Otsu_opencv(SDL_Surface * surface)
Uint32 pixel_to_BlackAndWhite(Uint32 pixel_color, SDL_PixelFormat* format, int tresh)
void surface_to_BlackAndWhite(SDL_Surface* surface)
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
void surface_to_grayscale(SDL_Surface * surface)
