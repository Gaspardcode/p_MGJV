
typedef struct {
 »·······int x;
 »·······int y;
 }P;
 typedef struct {
 »·······P p1;
 »·······P p2;
 }L;
int * flood_fill_rec(int i, int * dim ,int * isl)
{
	//coo has len 4 
	//modifies coo as : {lowest x, lowest y, max y, size of the island}
	//recursive
	
	//must go to iterative
	//flood
	isl[i] = 0;
	int * catch;
	int * res = malloc(4*sizeof(int));
	res[3] = 1;
	//dim[0] = width of isl
	res[0] = i / dim[0];
	res[1] = i % dim[0];
	res[2] = 0;
	
	int dn = i - dim[0];
	int up = i + dim[0];
	int r = i + 1;
	int l = i - 1;
	int w = dim[0];

	printf("hello %d\n",i);
	if(dn >= 0 && dn < dim[1] && isl[dn] > 0)
	{
		catch = flood_fill(dn,dim,isl);
		if(catch[0]*w+catch[1] < res[0]*w +res[1])
		{
			res[0] = catch[0]; // new lowest x1
			res[1] = catch[1]; // new lowest y1
		}
		else if(catch[0] == res[0] && catch[2] > res[2])
			res[2] = catch[2]; // x1 && new highest y2
		res[3] += catch[3];	
	}
	if(up >= 0 && up < dim[1] && isl[up] > 0)
	{
		catch = flood_fill(up,dim,isl);
		if(catch[0]*w +catch[1] < res[0]*w+res[1])
		{
			res[0] = catch[0];
			res[1] = catch[1];
		}
		else if(catch[0] == res[0] && catch[2] > res[2])
			res[2] = catch[2];
		res[3] += catch[3];	
	}
	if(r >= 0 && r < dim[1] && isl[r] > 0)
	{
		catch = flood_fill(r,dim,isl);
		if(catch[0]*w+catch[1] < res[0]*w+res[1])
		{
			res[0] = catch[0];
			res[1] = catch[1];
		}
		else if(catch[0] == res[0] && catch[2] > res[2])
			res[2] = catch[2];
		res[3] += catch[3];	
	}
	if(l >= 0 && l < dim[1] && isl[l] > 0)
	{
		catch = flood_fill(l,dim,isl);
		if(catch[0]*w+catch[1] < res[0]*w+res[1])
		{
			res[0] = catch[0];
			res[1] = catch[1];
		}
		else if(catch[0] == res[0] && catch[2] > res[2])
			res[2] = catch[2];
		res[3] += catch[3];	
	}
	return res;
}

/*
P * line_sect(L * line1, L * line2)
{
	//double r1 = (line1->p2.y - line1->p1.y) / (line1->p2.x - line1->p1.x);
	//double r2 = (line2->p2.y - line2->p1.y) / (line2->p2.x - line2->p1.x);
	double t1 = 
	double t2 = ;
	double a = ;
	double b = ;
	return &(line1->p1);
}
*/

int * line_inter(double a1, double a2, double b1, double b2,
		double c1, double c2)
{
	/*Cramer's method
	int m1[4] = {r1, t1
		     r2 t2};
	int m2[4] = {a, t1
		     b, t2 };
	int m3[4] = {r1, a
		     r2, b};
	determinant solving*/

	double d1 = a1*b2 - a2*b1;
	double d2 = c1*b2 - b1*c2;
	double d3 = a1*c2 - c1*a2;
	printf("d1:%lf d2:%lf d3:%lf\n",d1,d2,d3);
	if(d1 == 0)
		printf("lines are parallel\nstopping\n");
	double x = d2/d1;
	double y = d3/d1;
	int * cood = malloc(2*sizeof(int));
	cood[0] = round(x);
	cood[1] = round(y);
	return cood;
}
int * integral_image(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w*h;
	int* integral = calloc(len,sizeof(int));
	integral[0] = pixels[0];
	for(int i = 1; i < w; i++)
		integral[i] = integral[i-1] + pixels[i];
	for(int j = 1; j < h; j++)
		integral[j*w] = integral[(j-1)*w] + pixels[j*w];
	for(int i = 1; i < h; i++)
		for(int j = 1; j < w; j++)
		{
			int ind = i*w + j;
			int up = i - w;
			int left = i - 1;
			int ul = i - w - 1;
			int val = pixels[ind];
			integral[ind] = integral[up] + integral[left]
				+ integral[ul] + val;
		}
	return integral;
}
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
void Draw_Line(SDL_Surface * surface, double m, double b)
{
	double x,y;
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int X,Y;
	printf("Line : m : %lf b :%lf\n",m,b);
	SDL_LockSurface(surface);

	for(x = 0; x < w; x+= 0.01)
	{
		y = x*m + b;
		X = (int)x;
		Y = (int)y;
		if(X >= 0 && X < w && Y >= 0 && Y < h)
			pixels[Y*w+X] = 0xFFFFFF;
	}

	SDL_UnlockSurface(surface);

}
void delete_k_neigh(int * arr, int k, int rho, int theta ,int w, int h)
{
	int start = theta - k;
	int end = theta + k;
	for(int i = rho - k ; i < rho + k; i++)
	{	
		for(int j = start; j < end; j++)
		{
			int ind = i*w+j;
			if(ind > 0 && ind < w*h)
				arr[ind] = 0;
		}
	}
}
void surface_to_grid(SDL_Surface * surface)
{
	Uint32* pixels = surface->pixels;
	const int w = surface->w;
	const int h = surface->h;
	SDL_LockSurface(surface);
	
	// shifted angle range
	const int a_r = 180;
	// angle range
	const int diag = (int)sqrt(w*w + h*h);

	// [-diag,diag] into a 2 * diag array
	const int sizeR = 2 * diag;
	const int nlen = sizeR * a_r;
	int * h_plane = calloc(nlen, sizeof(int));
	
	//pre computation
	double * coss = calloc(a_r,sizeof(double));
	double * sins = calloc(a_r,sizeof(double));
	for(int theta = 0 ; theta < a_r; theta++)
	{
		double t_rad = (double)theta*(PI/180.);
		coss[theta] = cos(t_rad);
		sins[theta] = sin(t_rad);
	}

	int max = 0;
	int rho, theta, p;
	double r;

	for(int x = 0; x < h; x++)
	{
		for(int y = 0; y < w; y++)
		{
			if(pixels[x*w + y] > 0)
			{
				for(theta = 0; theta < a_r; theta += 1)
				{
					r = y*coss[theta] + x*sins[theta];
					rho = (int)((r))+diag;
					p = rho*a_r + theta;
					if(rho >= 0 && rho < sizeR && p >= 0 && p < nlen)
					{
						h_plane[p]++;
						if(h_plane[p] > max)
							max = h_plane[p];
					}
				}
			}	
			// clear image to see hough space
			pixels[x*w + y] = 0;
		}
	}

	const int tresh = (int)(0.6 * max); //temporary
	int nbline = 0;
	int min_max[2] = {999999,0};
	int min_min[2] = {999999,999};
	int max_min[2] = {0,180};
	int max_max[2] = {0,0};

	for(int i = 1; i < sizeR-1 && nbline < 30; i++)
	{
		// Select local maxima with a treshold 
		// Extract lines
		for(theta = 0; theta < a_r ;theta++)
		{
			int ind = i * a_r  + theta;
			int up = ind - a_r;
			int down = ind + a_r;
			int left = ind - 1;
			int right = ind + 1;
			int upr = up + 1;
			int upl = up - 1;
			int dl = down - 1;
			int dr = down + 1;
			int v = 0;
			if(ind < nlen)
				v = h_plane[ind]-1;
			if(v > tresh

			&& up >= 0 && up < nlen 
			&& down >= 0 && down < nlen
			&& left >= 0 && left < nlen
			&& right >= 0 && right < nlen
			&& upl >= 0 && upl < nlen
			&& upr >= 0 && upr < nlen
			&& dl >= 0 && dl < nlen
			&& dr >= 0 && dr < nlen

			&& v >= h_plane[up] && v >= h_plane[down]
			&& v >= h_plane[left] && v >= h_plane[right]
			&& v >= h_plane[upl] && v >= h_plane[upr]
			&& v >= h_plane[dl] && v >= h_plane[dr])
  
			{
				nbline++;
				if((theta <= 35 || theta >= 55) && (theta <= 120 || theta >= 150))
				{
					rho = i - diag;
					if(rho < min_min[0] && theta < min_min[1])
					{
						min_min[0] = rho;
						min_min[1] = theta;
					}
					else if(rho < min_max[0] && theta > min_max[1])
					{
						min_max[0] = rho;
						min_max[1] = theta;
					}
					else if(rho > max_min[0] && theta < max_min[1])
					{
						max_min[0] = rho;
						max_min[1] = theta;
					}
					else if(rho > max_max[0] && theta > max_max[1])
					{
						max_max[0] = rho;
						max_max[1] = theta;
					}
				}
				delete_k_neigh(h_plane,diag/20,i,theta,a_r,sizeR);
			}
		}
	}

	Draw_Polar_Line(surface,(double)max_max[0]/2,coss[max_min[1]],sins[max_min[1]]);
	Draw_Polar_Line(surface,(double)max_min[0],coss[max_min[1]],sins[max_min[1]]);
	Draw_Polar_Line(surface,(double)min_min[0],coss[min_min[1]],sins[min_min[1]]);
	Draw_Polar_Line(surface,(double)max_max[0],coss[max_max[1]],sins[max_max[1]]);
	printf(" line min_max:rho:  %d theta:%d\n",min_max[0],min_max[1]);
	printf(" line max_min:rho:  %d theta:%d\n",max_min[0],max_min[1]);
	printf(" line min min:rho:  %d theta:%d\n",min_min[0],max_min[1]);
	printf(" line max max:rho:  %d theta:%d\n",max_max[0],max_max[1]);
	printf("Detected line(s) : %d\n",nbline);

//crop square grid from upper left corner
//	SDL_Rect src = {ul,ur,side,side};
//	SDL_Rect dst = {0,0,w,h};
//	SDL_BlitSurface(sco,src,sco,dst);

	free(h_plane);
	free(coss);
	free(sins);
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
		theta = 2.0*PI*8;
		printf("rho: %lf theta: %lf  %d\n",rho,theta,x);
		Draw_Polar_Line(surface, rho, cos(theta), sin(theta));
	}	
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
int Gaussian_by_five[25] = {   1,    4,   7,   4, 1,
                      		 4,   16,  26,  16, 4,
                      		 7,   26,  41,  26, 7,
                      		 4,   16,  26,  16, 4,
		      		 1,    4,   7,   4, 1    };

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
