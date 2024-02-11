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
#define MASK_LEN_GAUSS_5 25
#define MASK_LEN_GAUSS_7 49
#define KERNEL_SIZE 2
#define PI 3.141592653589793238462
int * flood_debug(int i, int * dim, int *isl, int *fld);
int * flood_fill(int i, int *dim, int *isl);
int * flood_print(int i, int *dim, int *isl,int color);
void gray(SDL_Surface * sco);
void speed_ada(SDL_Surface * sco, int size);
void extract_grid(SDL_Surface **sco);
int * line_inter(double a1, double a2, double b1, double b2, double c1, double c2);
void integral_image(SDL_Surface *surface, int * integral);
void surface_to_rotate_shear(SDL_Surface ** surface, const int theta);
int max_arr(int* array, int len);
void Draw_Polar_Line(SDL_Surface * surface, double r, double ct, double st);
void delete_k_neigh(int * arr, int k, int rho, int theta ,int w, int h);
void surface_to_hough(SDL_Surface * surface, int * min_theta);
void surface_to_adaptive_treshold(SDL_Surface * surface, int size);
void surface_to_erosion(SDL_Surface * surface);
void surface_to_dilatation(SDL_Surface * surface);
void surface_to_opening(SDL_Surface * surface);
void surface_to_closing(SDL_Surface * surface);
void draw(SDL_Renderer* renderer, SDL_Texture* texture);
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture *treatment);
SDL_Surface* load_image(const char* path);
double convolution(Uint8* pixels, const int *mask, int len);
double pixel_to_blur(unsigned char * pixels, int size);
int pixel_to_angle(Uint8* pixels);
double sobel_magnitude(Uint8* pixels);
void surface_to_blur(SDL_Surface * surface, int kernel_size);
void surface_to_canny(SDL_Surface * surface);
int min(Uint8* array, int start);
int min_n(Uint8* array, int len);
int max_n(Uint8* array, int len);
void sort(Uint8* medians);
Uint32 pixel_to_median(Uint32* pixels,Uint32 pixel_color,
SDL_PixelFormat* format, int i, int len, int w);
void surface_to_median(SDL_Surface* surface);
Uint32 pixel_to_invert(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_invert(SDL_Surface* surface);
long double w1_t(int t, int n, int * histogram);
long double w2_t(int t, int n, int * histogram);
long double P_i(int i, int n, int * histogram);
long double u1_t(int t, int n, int * histogram);
double u2_t(int t, int n, int * histogram);
long double sigma_b_t(int t, int n, int * histogram);
int Otsu(SDL_Surface * surface);
int Otsu_opencv(SDL_Surface * surface);
Uint32 pixel_to_BlackAndWhite(Uint32 pixel_color, SDL_PixelFormat* format, int tresh);
void surface_to_BlackAndWhite(SDL_Surface* surface);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface * surface);
