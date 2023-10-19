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
#define T(a,b,t) ((a) - (b)) > (c) 
void surface_to_resize(SDL_Surface * surface, const double scale)
{ 
	// downsize only
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int nw = scale * w;
	int nh = scale * h;
	printf("%d %d\n", nw ,nh);
	unsigned char * resize = calloc(nw*nh,1); 
	double x_r = w /(double)nw;
	double y_r = h /(double)nh;
	double x,y;
	for(int i = 0; i < nh; i++)
	{
		for(int j = 0; j < nw; j++)
		{
			x = floor(i*y_r);
			y = floor(j*x_r);
			if(pixels[(int)(x*w + y)] > 0)
				resize[(i*nw)+j] = 0xFF;
		}
	}
	for(int i = 0; i < nh; i++)
	{
		for(int j = 0; j < nw; j++)
		{
			pixels[i*w + j] = (resize[i*nw + j] > 0) ? 0xFFFFFFFF : 0;
		}	
	}
	SDL_UnlockSurface(surface);
	free(resize);

	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
void surface_to_resize_border(SDL_Surface ** surface, const int nw, const int nh)
{
	/*
	Uint32 flags = (*surface)->flags;
	SDL_Format format = (*surface)->format;
	int depth = format->BitsPerPixel;
	SDL_Surface *surf = SDL_CreateRGBSurface(flags,nw,nh,depth,0,0,0,0);
	SDL_Rect crop = {0,0,nw,nh};
	SDL_BlitSurface(*surface,&crop,surf,0);
	*/
	//SDL_Surface *surf = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
	Uint32* pixels = (*surface)->pixels;

	int * pix = calloc(nw*nh,8);
	SDL_LockSurface(*surface);
	for(int i = 0; i < nw * nh ; i++)
		pix[i] = (pixels[i] > 0) ? 0xFFFF : 0;	
	SDL_UnlockSurface(*surface);

	SDL_Surface *surf = SDL_CreateRGBSurfaceFrom(pix,nw,nh,16,nw*4,0,0,0,0);
	SDL_FreeSurface(*surface);
	*surface = surf;
	free(pixels);
}
void surface_to_rotate_shear(SDL_Surface * surface, const int theta)
{
	// rotates counterclockwise
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	const double r_t = (double)theta * (PI/180.);
	double t = tan(r_t/2.);
	double s = sin(r_t);
	//double c = cos(r_t);
	//int nw = abs((int)(w*c)) + abs((int)(h*s));
	//int nh = abs((int)(h*c)) + abs((int)(w*s));
	SDL_LockSurface(surface);
	unsigned char * bigger = calloc(len,1);//sizeof(char) = 1
	for(int i = 0; i < len; i++)
	{
		int x = i / w;
		int y = i - (x * w);
		int d_x = x - h/2;
		int d_y = y - w/2;
		int nx = d_x - d_y*t;//first shear
		int ny = nx*s + d_y + w/2;//second shear
		nx = nx - (ny-w/2)*t + h/2; //third shear
		if(nx >= 0 && nx < h && ny >= 0 && ny < w)
		{	
			if(pixels[i] > 0)
				bigger[nx*w + ny] = 0xFF;
		}
	}
	/*
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	free(pixels);

	surface = SDL_CreateRGBSurface(0,nw,nh,32,0,0,0,0);
	pixels = surface->pixels;
	
	SDL_LockSurface(surface);
	*/
	for(int i = 0; i < len; i++)
		pixels[i] = (bigger[i] > 0) ? 0xFFFFFFFF : 0;
	SDL_UnlockSurface(surface);
	free(bigger);

	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
//SDL_Surface * surface_to_rotate(SDL_Surface * surface, int theta)
void surface_to_rotate(SDL_Surface * surface,const int theta)
{
	// rotates counterclockwise
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	//int b = (w + h)/3; // border
	//int nlen = (w + b) * (h + b);
	const double r_t = (double)theta * (PI/180.);
	double c = cos(r_t);
	double s = sin(r_t);
	SDL_LockSurface(surface);
	unsigned char * bigger = calloc(len,1);//sizeof(char) = 1
	for(int i = 0; i < len; i++)
	{
		int x = i / w;
		int y = i - (x * w);
		int d_x = x - h/2;
		int d_y = y - w/2;
		//int nx = x*c - y*s + h/2;
		//int ny = x*s + y*c + w/2;
		int nx = d_x*c - d_y*s + h/2;
		int ny = d_x*s + d_y*c + w/2;
		if(nx >= 0 && nx < h && ny >= 0 && ny < w)
		{	
			if(pixels[i] > 0)
				bigger[nx*w + ny] = 0xFF;
		}
	}
/*		
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	
	SDL_Surface * surface_b = SDL_CreateRGBSurfaceFrom(bigger,w+b,h+b,32,w+b,0,0,0,0);
	free(bigger);
	if(surface_b == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
		
	unsigned char * p = surface_b->pixels;
	for(int i = 0; i < nlen; i++)
	{
		printf("%X ",p[i]);
	}
*/		
	for(int i = 0; i < len; i++)
		pixels[i] = (bigger[i] > 0) ? 0xFFFFFFFF : 0;

	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
	
	//return surface_b;
}
int max_arr(int* array, int len)
{
	int max = 0;
	int index = 0;
	for(int i = 0; i < len; i++)
	{
		index = array[i] > max ? i : index;
		max = array[i] > max ? array[i] : max;
	}
	return index;
}
void Draw_Polar_Line(SDL_Surface * surface, double r, double ct, double st)
{
	double x,y;
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int X,Y;
	printf("Polar_Lines : rho : %lf theta :%lf\n",r,acos(ct));
	SDL_LockSurface(surface);

	for(x = 0; x < w; x+= 0.01)
	{
		y = (r/st)-x*(ct/st);
		X = (int)x;
		Y = (int)y;
		if( X >= 0 && X < w && Y >= 0 && Y < h)
			pixels[Y*w+X] = 0xFFFFFF;
	}

	SDL_UnlockSurface(surface);

}
void surface_to_hough(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	const int w = surface->w;
	const int h = surface->h;
	const int len = w * h;
	SDL_LockSurface(surface);
	
	// shifted angle range
	const int a_r = 360;
	// angle range
	const int diag = (int)sqrt(w*w + h*h);

	// [-diag,diag] into a 2 * diag array

	const int sizeR = diag;
	const int nlen = sizeR * a_r;
	
	int * h_plane = calloc(nlen, sizeof(int));
	
	//pre computation
	double * coss = calloc(a_r,sizeof(double));
	double * sins = calloc(a_r,sizeof(double));
	for(int theta = 0 ; theta < a_r; theta++)
	{
		double t_rad = ((double)theta)*(PI/180.);
		coss[theta] = cos(t_rad);
		sins[theta] = sin(t_rad);
	}
	printf("%d*%d  = %d ; %d \n",w,h,len,nlen);
	int max = 0;
	int rho, theta, p;
	double r;

	for(int x = 0; x < h; x++)
	{
		for(int y = 0; y < w; y++)
		{
			if(pixels[x*w + y] > 0)
			{
				for(theta = 0; theta < a_r; theta += 3)
 				{
					r = x*coss[theta] + y*sins[theta];
					rho = (int)((r));//(double)diag)*(double)sizeR);
					p = rho*sizeR + theta;
					if(rho >= 0 && rho < sizeR && p >= 0 && p < nlen)
					{
						++h_plane[p];
						if(h_plane[p] > max)
							max = h_plane[p];
					}
				}
			}	
			// clear image to see hough space
			pixels[x*w + y] = 0;
		}
	}

	const int tresh = (int)(0.5 * max); //temporary
	const int t = 0;
	int nbline = 0;
	int * rhos = calloc(1000,sizeof(int));
	int * thetas = calloc(1000,sizeof(int));
	int * val = calloc(1000,sizeof(int));
	for(int i = 1; i < sizeR-10 && nbline < 100; i++)
	{
		// Select local maxima with a treshold 
		// Extract lines
		for(theta = 1; theta < a_r - 10;theta++)
		{
			int ind = i * sizeR + theta;
			int up = ind - sizeR;
			int down = ind + sizeR;
			int left = ind - 1;
			int right = ind + 1;
			int upr = up + 1;
			int upl = up - 1;
			int dl = down - 1;
			int dr = down + 1;
			int v = h_plane[ind];
			if(v > tresh
			&& up >= 0 && up < nlen 
	/*
			&& down >= 0 && down < nlen
			&& left >= 0 && left < nlen
			&& right >= 0 && right < nlen
			&& upl >= 0 && upl < nlen
			&& upr >= 0 && upr < nlen
			&& dl >= 0 && dl < nlen
			&& dr >= 0 && dr < nlen
	*/
			&& (v - h_plane[up]) >= t && (v - h_plane[down]) >= t
			&& (v - h_plane[left]) >= t && (v - h_plane[right]) >= t
			&& (v - h_plane[upl]) >= t && (v - h_plane[upr]) >= t
			&& (v - h_plane[dl]) >= t && (v - h_plane[dr]) >= t)
			{
				++nbline;
				r = diag*((double)i/(double)sizeR); 
				rhos[nbline-1] = r;
				thetas[nbline-1] = theta;
				val[nbline-1] = v;	       		
				//Draw_Polar_Line(surface,
				//	(double)r,coss[theta],sins[theta]);
			}
		}
	}
	
	int ind = 0;
	for (int i = 0; i < 10 && nbline > 0; i++,nbline--)
	{
		ind = max_arr(val,1000);
		val[ind] = 0;
		Draw_Polar_Line(surface,
			(double)rhos[ind],coss[thetas[ind]],sins[thetas[ind]]);
	}	
	printf("Detected line(s) : %d\n",nbline);

	// h_plane holds the hough space; the accumulator
	free(h_plane);
	free(coss);
	free(sins);
	free(rhos);
	free(thetas);
	//free(lines);
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
void RandomLines(SDL_Surface * surface, int n)
{
	int w = surface->w;
	int h = surface->h;
	double rho, theta;

	for(int i=0; i < n; i++)
	{
		int x = i*i*i;
		rho = (int)(x*sqrt(w*w+h*h)) % w;
		theta = 2.0*PI*random();
		printf("rho: %lf theta: %lf  %d\n",rho,theta,x);
		Draw_Polar_Line(surface, rho, cos(theta), sin(theta));
	}	
}
void surface_to_adaptive_treshold(SDL_Surface * surface, int size)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	
	//computes grayscale
	Uint8 grayscale[len];
	for(int i = 0; i < len; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i],format, &r, &g, &b);
		float average = 0.3*r + 0.59*g + 0.11*b;
		grayscale[i] = (Uint8)average;
	}
	
	// cumptes treshold based on neighbours 
	for(int i = 0; i < len; i++)
	{
		int sum = 0;
		for(int u = (-1)*size ; u < size+1 ; u++)
		{
			for(int bias = (-1)*size; bias < size+1; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
					sum += grayscale[j];
				else
					sum += grayscale[i];
			}
		}
		int tresh = sum /((size*2+1)*(size*2+1));
		pixels[i] = (grayscale[i] >= tresh) ? 0xFFFFFF : 0x0 ;
	}

	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}

void surface_to_erosion(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	SDL_LockSurface(surface);
	
	int * erode = (int*)malloc(len*sizeof(int));
	for(int i = 0; i < len; i++)
		erode[i] = pixels[i];
	// erosion process
	for(int i = 0; i < len; i++)
	{
	// does not trigger the loop if erode[i] is 0
		for(int u = -1 ; u < 2 && erode[i] ; u++)
		{
			for(int bias = -1; bias < 2 && erode[i]; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
	//!pixels[j] stands for : there is a black pixel in the neighbours
					erode[i] = !pixels[j] ? BLACK : 0xFFFFFF ;
			}
		}
	}
	for(int i = 0; i < len; i++)
		pixels[i] = erode[i];

	free(erode);
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}


void surface_to_dilatation(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	SDL_LockSurface(surface);
	
	int * erode = (int*)malloc(len*sizeof(int));
	for(int i = 0; i < len; i++)
		erode[i] = pixels[i];
	// erosion process
	for(int i = 0; i < len; i++)
	{
	// does not trigger the loop if erode[i] is 0
		for(int u = -1 ; u < 2 && !erode[i] ; u++)
		{
			for(int bias = -1; bias < 2 && !erode[i]; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
	//pixels[j] stands for : there is a white pixel in the neighbours
					erode[i] = pixels[j] ? 0xFFFFFF : BLACK;
			}
		}
	}
	for(int i = 0; i < len; i++)
		pixels[i] = erode[i];

	free(erode);
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}

void surface_to_opening(SDL_Surface * surface)
{
	surface_to_erosion(surface);
	surface_to_dilatation(surface);
}
void surface_to_closing(SDL_Surface * surface)
{
	surface_to_dilatation(surface);
	surface_to_erosion(surface);
}
// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* treatment)
{
    SDL_Event event;
    SDL_Texture* t = treatment;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
		case SDL_QUIT:
			return;
		case SDL_WINDOWEVENT:
			if(event.window.event == SDL_WINDOWEVENT_RESIZED)
				draw(renderer, t);
			break;
		case SDL_KEYDOWN:
			if(t == colored)
				t = treatment;
			else
				t = colored;
			draw(renderer,t);
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
	SDL_Surface* surface = IMG_Load(path);
	SDL_Surface* surf = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(surface);
	if(surf == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
	return surf;
}

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
{
	double convol = 0;
	
for(int i = 0; i < len; i++)
	convol += mask[i] * (double)pixels[i];

return convol;
}
double pixel_to_blur(Uint8* pixels)
{
double G = convolution(pixels, Gaussian_sigma_one, MASK_LEN_GAUSS);

//return (long double)(1./159.) * G;
return (long double)(1./273.) * G;
}
int pixel_to_angle(Uint8* pixels)
{
double Gx = (1./9.) * convolution(pixels, sobel_Gx, MASK_LEN_SOBEL);
double Gy = (1./9.) * convolution(pixels, sobel_Gy, MASK_LEN_SOBEL);

double deg_convert = 180.000 / PI;
double angle = atan2(Gy,Gx) * deg_convert;
angle = (angle > 0) ? angle : (-1.0) * angle;

	// rounds the angle to four directions
	// 0 - 45 - 90 - 135
	double r_angle ;

	if(angle >= 0.000 && angle < 22.500)
		r_angle = 0;
	if(angle >= 157.500 && angle < 180.000)
		r_angle = 0;
	if(angle >= 22.500 && angle < 76.500)
		r_angle = 45;	
	if(angle >= 76.500 && angle < 112.500)
		r_angle = 90;
	else
		r_angle = 135;

	return r_angle;
}
double sobel_magnitude(Uint8* pixels)
{
	double Gx = (1./9.) * convolution(pixels, sobel_Gx, MASK_LEN_SOBEL);
	double Gy = (1./9.) * convolution(pixels, sobel_Gy, MASK_LEN_SOBEL);

	return (sqrt(Gx*Gx + Gy*Gy));
}
void surface_to_blur(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	
	//computes grayscale
	Uint8 grayscale[len];
	for(int i = 0; i < len; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i],format, &r, &g, &b);
		float average = 0.3*r + 0.59*g + 0.11*b;
		grayscale[i] = (Uint8)average;
	}
	
	// computes gaussian noise filter
	for(int i = 0; i < len; i++)
	{
		Uint8 neighb[25];
		int k = 0;
		for(int u = -2 ; u < 3 ; u++)
		{
			for(int bias = -2; bias < 3; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
					neighb[k] = grayscale[j];
				else
					neighb[k] = grayscale[i];
				k++;
			}
		}
		grayscale[i] = (unsigned int)(pixel_to_blur(neighb));
	}

	for(int i = 0; i < len ; i++)
	{
		Uint32 color = SDL_MapRGB(format, grayscale[i],grayscale[i],grayscale[i]);
		pixels[i] = color;
	}

	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
void surface_to_canny(SDL_Surface * surface)
{
	// Applies a Gaussian filter to reduce noise
	surface_to_blur(surface);

	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);

	double* sob_m = (double*)malloc(len * sizeof(double));
	int* sob_a = (int*)malloc(len * sizeof(int));
	int* sob = (int*)malloc(len * sizeof(int));

	if(sob_m == NULL || sob == NULL || sob_a == NULL)
        	errx(EXIT_FAILURE,"%s", "Not enough memory\n");
	for(int i = 0; i < len; i++)
	{
		sob[i] = pixels[i] >> 16;
	       	sob_a[i] = sob_m[i] = 0;	
	}

	// computes sobel magintude matrix
	// computes sobel angles matrix
	for(int i = 0; i < len; i++)
	{
		Uint8 neighb[9];
		int k = 0;
		for(int u = -1; u < 2 ; u++)
		{
			for(int bias = -1; bias < 2; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
					neighb[k] = sob[j];
				else
					neighb[k] = sob[i];
				k++;
			}
		}
		sob_m[i] = sobel_magnitude(neighb);
		sob_a[i] = pixel_to_angle(neighb);
	}
	
	// Angle selection according to the magnitude of neighb pixels
	// At the same time Double Treshold
	int low_tresh = 0;
	int high_tresh = 100;
	for(int i = 0; i < len ; i++)
	{
		if(sob_a[i] == 0)
		{
			int j1 = i - 1;
		        int j2 = i + 1;
			if(j1 >= 0 && j2 < len)
			{
				sob[i] = (sob_m[i] > sob_m[j1] && sob[i] > sob_m[j2])
				? sob[i] : 0;  			
			}
		}
		if(sob_a[i] == 45)
		{
			int j1 = i - w + 1;
		        int j2 = i + w - 1;
			if(j1 >= 0 && j2 < len)
			{
				sob[i] = (sob_m[i] > sob_m[j1] && sob[i] > sob_m[j2])
				? sob[i] : 0;  			
			}
		}
		if(sob_a[i] == 90)
		{
			int j1 = i - w;
		        int j2 = i + w;
			if(j1 >= 0 && j2 < len)
			{
				sob[i] = (sob_m[i] > sob_m[j1] && sob[i] > sob_m[j2])
				? sob[i] : 0;  			
			}
		}
		if(sob_a[i] == 135)
		{
			int j1 = i - w - 1;
		        int j2 = i + w + 1;
			if(j1 >= 0 && j2 < len)
			{
				sob[i] = (sob_m[i] > sob_m[j1] && sob[i] > sob_m[j2])
				? sob[i] : 0;  			
			}
		}	
	//Double Treshold
	//Keeping strong and weak edges above the treshold
		sob[i] = (sob[i] >= low_tresh) ? sob[i] : 0; 
	}

	free(sob_m);
	free(sob_a);
	
	/*
	 Quick normalization
	for(int i = 0; i < len; i++)
	{
		sob[i] = (sob[i] < 255) ? sob[i] : WHITE;
	       	sob[i] = (sob[i] > 0) ? sob[i] : BLACK;	
	}
	*/
	//Hysteresis
	for(int i = 0; i < len; i++)
	{
		if(sob[i] >= low_tresh && sob[i] < high_tresh)
		{
			int keep = 0;
			int k = 0;
			for(int u = -1; !keep && u < 2 ; u++)
			{
				for(int bias = -1; !keep && bias < 2; bias++)
				{
					int j = i + u * w + bias;
					if(j >= 0 && j < len && sob[j] > high_tresh)
						keep = 1;
					k++;
				}
			}
	// putting away weak edges that do not meet a strong edge in their neighbs
			sob[i] = (keep) ? sob[i] : 0;
		}
	}
	
	// Final surface filling
	for(int i = 0; i < len ; i++)
	{
		Uint32 color = SDL_MapRGB(format, sob[i], sob[i],sob[i]);
		pixels[i] = color;
	}

	free(sob);

	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
/*
void surface_to_sobel(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	
	//computes grayscale
	Uint8 grayscale[len];
	for(int i = 0; i < len; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i],format, &r, &g, &b);
		float average = 0.3*r + 0.59*g + 0.11*b;
		grayscale[i] = (Uint8)average;
	}
	
	// computes sobel filter
	for(int i = 0; i < len; i++)
	{
		Uint8 neighb[9];
		int k = 0;
		for(int u = -1; u < 2 ; u++)
		{
			for(int bias = -1; bias < 2; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
					neighb[k] = grayscale[j];
				else
					neighb[k] = grayscale[i];
				k++;
			}
		}
		grayscale[i] = (1./9.) * pixel_to_sobel(neighb) 
	}

	for(int i = 0; i < len ; i++)
	{
		Uint32 color = SDL_MapRGB(format, grayscale[i],grayscale[i],grayscale[i]);
		pixels[i] = color;
	}

	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
*/
///min of a 9 element 
//array starting at index start
int min(Uint8* array, int start)
{
	Uint32 min = MAX_PIX_VAL;
	int index = 0; 
	for(int i = start; i < 9; i++)
	{
		index = array[i] < min ? i : index;
		min = array[i] < min ? array[i] : min;
	}
	return index;
}
//min of a len element array of pixels
int min_n(Uint8* array, int len)
{
	Uint32 min = MAX_PIX_VAL;
	int index = 0;
	for(int i = 0; i < len; i++)
	{
		index = array[i] < min ? i : index;
		min = array[i] < min ? array[i] : min;
	}
	return index;
}
//max of a len element array of pixels
int max_n(Uint8* array, int len)
{
	Uint32 max = 0;
	int index = 0;
	for(int i = 0; i < len; i++)
	{
		index = array[i] > max ? i : index;
		max = array[i] > max ? array[i] : max;
	}
	return index;
}
//Normalizes a pixel
Uint32 pixel_to_normalize(Uint32 pixel_color, SDL_PixelFormat* format, int min_r, int max_r,int min_g,int max_g,int min_b,int max_b)
{
	Uint8 r, g, b;
	SDL_GetRGB(pixel_color,format, &r, &g, &b);
	r = 255 * (r-min_r)/(max_r-min_r);	
	g = 255 * (g-min_g)/(max_g-min_g);	
	b = 255 * (b-min_b)/(max_b-min_b);		
	Uint32 color = SDL_MapRGB(format, r, g, b);
	return color;

}
void surface_to_normalize(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	
	// step for min_max values of each channel 
	int i = 0;
	Uint8 min_max_r[len];
	Uint8 min_max_g[len];
	Uint8 min_max_b[len];
	while(i < len)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i],format, &r, &g, &b);
		min_max_r[i] = r;
		min_max_g[i] = g;
		min_max_b[i] = b;
		i++;
	}
	i = 0;
	int min_r = min_n(min_max_r, len);
	int max_r = max_n(min_max_r, len);
	int min_g = min_n(min_max_g, len);
	int max_g = max_n(min_max_g, len);
	int min_b = min_n(min_max_b, len);
	int max_b = max_n(min_max_b, len);
	// end of the steps
	while(i < len)
	{
		pixels[i] = pixel_to_normalize(pixels[i], format, min_r,max_r,min_g,max_g,min_b,max_b);
		i++;
	}
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
// selection sort on an 9 element array
void sort(Uint8* medians)
{
	for(int i = 0; i < 9; i++)
	{
		Uint8 swap_val = medians[i];
		int min_index = min(medians,i);
		medians[i] = medians[min_index];
		medians[min_index] = swap_val;
	}	
}
// Applies median filter to a colored pixel
Uint32 pixel_to_median(Uint32* pixels,Uint32 pixel_color, SDL_PixelFormat* format, int i, int len, int w)
{
	Uint8 r, g, b;
	Uint8 medians_r[9];
	Uint8 medians_g[9];
	Uint8 medians_b[9];
	medians_r[0] = medians_g[0] = medians_b[0] = 0;
	int k = 0;
	for(int u = -1; u < 2 ; u++)
	{
		int biais = -1;
		while(biais < 2)
		{
			int j = i + u * w + biais;
			if(j >= 0 && j < len)
			{
				SDL_GetRGB(pixels[j],format, &r, &g, &b);
				medians_r[k] = r;
				medians_g[k] = g;
				medians_b[k] = b;
			}
			else
				return pixel_color;
			biais++;
			k++;
		}
	}
	sort(medians_r);
	sort(medians_g);
	sort(medians_b);
	Uint32 color = SDL_MapRGB(format, medians_r[4],medians_g[4],medians_b[4]);
	return color;
}
//Applies median filter to a image
void surface_to_median(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	int i = 0;
	while(i < len)
	{
		pixels[i] = pixel_to_median(pixels,pixels[i], format,i,len,surface->w);
		i++;
	}
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
Uint32 pixel_to_invert(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;
	SDL_GetRGB(pixel_color,format, &r, &g, &b);
	r = 255 - r;
	g = 255 - g;
	b = 255 - b;
	Uint32 color = SDL_MapRGB(format, r, g, b);
	return color;
}
void surface_to_invert(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	for(int i = 0; i < len; i++)
		pixels[i] = pixel_to_invert(pixels[i], format);
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());

}
long double w1_t(int t, int n, int * histogram)
{
	long double w1 = 0;
	for(int i = 1; i <= t; i++)
		w1 += (long double)histogram[i] / (long double)n;
	return w1;
}
long double w2_t(int t, int n, int * histogram)
{
	long double w2 = 0;
	for(int i = t + 1; i < MAX_PIX_VAL ; i++)
		w2 += (long double)histogram[i] / (long double)n;
	return w2;
}
long double P_i(int i, int n, int * histogram)
{
	return (long double)histogram[i] / (long double)n;
}
long double u1_t(int t, int n, int * histogram)
{
	long double u1 = 0;
	for(int i = 1; i <= t; i++)
		u1 += ( (long double)i * P_i(i,n,histogram) ) / w1_t(i,n,histogram);
        return u1;	
}
double u2_t(int t, int n, int * histogram)
{
	long double u2 = 0;
	for(int i = t + 1; i < MAX_PIX_VAL; i++)
		u2 += ( (long double)i * P_i(i,n,histogram) ) / w2_t(i,n,histogram);
        return u2;	
}
long double sigma_b_t(int t, int n, int * histogram)
{
	long double factor = u1_t(t,n,histogram) - u2_t(t,n,histogram);
	return w1_t(t,n,histogram)*w2_t(t,n,histogram)*factor*factor;
}
int Otsu(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_LockSurface(surface);
	int histo[256];
	// Initializes the histogram
	for(int i = 0; i < MAX_PIX_VAL; i++)
		histo[i] = 0;
	
	for(int i = 0; i < len; i++)
		// Pixel is on 32 bits, pixel is grayscale at this point, we only need r
		histo[pixels[i] >> 16] += 1;

	SDL_UnlockSurface(surface);
/*	
	printf("%d\n", len);
	printf("%Lf\n", w1_t(2, len, histo));
	printf("%s", "-----------------------------------");
	for(int i = 0; i < MAX_PIX_VAL; i++)
		printf("%d : %d\n", histo[i], i);
	printf("%s", "-----------------------------------");
*/
	long double sigma_t;
	long double sigma_t_max = BLACK;
	for(int i = 0; i < MAX_PIX_VAL; i++)
	{
		sigma_t = sigma_b_t(i,len,histo);
		sigma_t_max = (sigma_t > sigma_t_max) ? sigma_t : sigma_t_max;
	}
//	printf("%Lf\n", sigma_t_max);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());

	return (int)(sigma_t_max);
}
int Otsu_opencv(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_LockSurface(surface);
	int histogram[256];
	// Initializes the histogram
	for(int i = 0; i < MAX_PIX_VAL; i++)
		histogram[i] = 0;
	
	for(int i = 0; i < len; i++)
		// Pixel is on 32 bits, pixel is grayscale at this point, we only need r
		histogram[pixels[i] >> 16] += 1;

	SDL_UnlockSurface(surface);
	
	// Calculate the bin_edges
	long double bin_edges[256];
	bin_edges[0] = 0.0;
	long double increment = 0.99609375;
	for(int i = 1; i < 256; i++)
    		bin_edges[i] = bin_edges[i-1] + increment;
 
	// Calculate bin_mids
	long double bin_mids[256];
	for(int i = 0; i < 255; i++)
  		bin_mids[i] = (bin_edges[i] + bin_edges[i+1])/2;
 
	// Iterate over all thresholds (indices) and get the probabilities weight1, weight2
	long double weight1[256];
	weight1[0] = histogram[0];
	for(int i = 1; i < 256; i++)
		weight1[i] = histogram[i] + weight1[i-1];
 
	int total_sum=0;
	for(int i = 0; i < 256; i++)
    		total_sum = total_sum + histogram[i];
	long double weight2[256];
	weight2[0] = total_sum;
	for(int i = 1; i < 256; i++)
  		weight2[i] = weight2[i-1] - histogram[i - 1];
 
	// Calculate the class means: mean1 and mean2
	long double histogram_bin_mids[256];
	for(int i = 0; i < 256; i++)
		histogram_bin_mids[i] = histogram[i] * bin_mids[i];
 
	long double cumsum_mean1[256];
	cumsum_mean1[0] = histogram_bin_mids[0];
	for(int i = 1; i < 256; i++)
		cumsum_mean1[i] = cumsum_mean1[i-1] + histogram_bin_mids[i];
 
	long double cumsum_mean2[256];
	cumsum_mean2[0] = histogram_bin_mids[255];
	for(int i = 1, j=254; i < 256 && j>=0; i++, j--)
		cumsum_mean2[i] = cumsum_mean2[i-1] + histogram_bin_mids[j];
 
	long double mean1[256];
	for(int i = 0; i < 256; i++)
		mean1[i] = cumsum_mean1[i] / weight1[i];
 
	long double mean2[256];
	for(int i = 0, j = 255; i < 256 && j >= 0; i++, j--)
		mean2[j] = cumsum_mean2[i] / weight2[j];
 
	// Calculate Inter_class_variance
	long double Inter_class_variance[255];
	long double dnum = 10000000000;
	for(int i = 0; i < 255; i++)
		Inter_class_variance[i] = ((weight1[i] * weight2[i] *
			(mean1[i] - mean2[i+1])) / dnum) * (mean1[i] - mean2[i+1]); 
 
	// Maximize interclass variance
	long double maxi = 0;
	int getmax = 0;
	for(int i = 0;i < 255; i++)
	{
		if(maxi < Inter_class_variance[i])
		{
		    maxi = Inter_class_variance[i];
		    getmax = i;
 	 	}
	}
	return (int)bin_mids[getmax];
}
// Converts a colored pixel into either black or white.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_BlackAndWhite(Uint32 pixel_color, SDL_PixelFormat* format, int tresh)
{
	// Assuming image is converted to grayscale
	Uint8 r, g, b;
	SDL_GetRGB(pixel_color,format, &r, &g, &b);
	if(r > tresh)
		r = g = b = WHITE;
	else
		r = g = b = BLACK;
	Uint32 color = SDL_MapRGB(format, r, g, b);
	return color;

}
void surface_to_BlackAndWhite(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	int i = 0;
	int tresh = Otsu_opencv(surface);
	int tresh_danger = Otsu(surface);
	printf("%d\n",tresh);
	printf("%d\n",tresh_danger);
	while(i < len)
	{
		pixels[i] = pixel_to_BlackAndWhite(pixels[i], format, tresh);
		i++;
	}
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
	Uint8 r, g, b;
	SDL_GetRGB(pixel_color,format, &r, &g, &b);
	float average = 0.3*r + 0.59*g + 0.11*b;
	int avg = (int)average;
	Uint32 color = SDL_MapRGB(format, avg, avg, avg);
	return color;

}
void surface_to_grayscale(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int len = surface->w * surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	for(int i = 0; i < len; i++)
		pixels[i] = pixel_to_grayscale(pixels[i], format);	
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

   // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Display", 0, 0, INIT_WIDTH, INIT_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the colored image.
    SDL_Surface* sco = load_image(argv[1]);
    //SDL_Surface* rot;

        // - Create a texture from the colored surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sco);

    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    if (BNW)
	    surface_to_BlackAndWhite(sco);
    if (BLACK)
	    surface_to_normalize(sco);
    for(int i = 0; i < NB_FILTER;i++)
    {
	    //surface_to_blur(sco);
	    //surface_to_adaptive_treshold(sco,1);
    	    //surface_to_sobel(sco);
	    //surface_to_grayscale(sco);
    	    //surface_to_invert(sco);
	    //surface_to_BlackAndWhite(sco);
	    //surface_to_canny(sco);
	    //surface_to_opening(sco);
	    //for(int i = 0; i < 1;i++)
	    //	surface_to_dilatation(sco);
	    //surface_to_closing(sco);
	    //surface_to_erosion(sco);
	    //surface_to_BlackAndWhite(sco);
	    //surface_to_median(sco);
	    /*
	    surface_to_resize(sco,252./1000.);
	    SDL_Rect src = {0,0,252,252};
	    SDL_Rect dst = {300,300,252,252};
	    SDL_BlitSurface(sco,&src,sco,&dst);
	    */
	    //surface_to_rotate(sco, 35);
	    //surface_to_rotate_shear(sco, 35);
	    //surface_to_resize_border(&sco,300,300);
	    //RandomLines(sco,25);
            surface_to_hough(sco);
	    //RandomLines(sco,100);
/* 
	    unsigned char * bigger = calloc(w*h,1);
	    for(int i = 0; i < w*h; i++)
		    bigger[i] = 0xFE;
	    for(int i = 0; i < 0; i++)
		    bigger[i] = 35;
	    sco = SDL_ConvertSurfaceFormat(sco, SDL_PIXELTYPE_ARRAYU8,0);
	    sco = SDL_CreateRGBSurfaceFrom(bigger,w,h,8,w,0,0,0,0);
	    free(bigger);
  	   if(sco->format->BitsPerPixel!=8)
        		errx(EXIT_FAILURE, "not 8 bits per pixel");
  	   if(sco->format->palette == NULL)
        		errx(EXIT_FAILURE, "not 8 bits per pixel");
	   printf("%d\n",sco->format->palette->ncolors);
*/
    }
    
    int w = sco->w;
    int h = sco->h;
    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, w, h);

    // - Create a new texture from the grayscale surface.
/*   
    for(int i = 0; i < 10000; i++) 
    {
	    SDL_RenderPresent(renderer);
    }
  */  
      SDL_Texture* t_gray = SDL_CreateTextureFromSurface(renderer, sco);

    // - Free the surface.
    SDL_FreeSurface(sco);
    //SDL_FreeSurface(rot);

    // - Dispatch the events.
    event_loop(renderer, texture, t_gray);

    // - Destroy the objects.

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
