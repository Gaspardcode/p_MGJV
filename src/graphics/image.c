#include "image.h"
#include "masks.c"
#include <math.h>
int grid_check(int x, int y,int * dim,int * isl)
{
	int w = dim[0];
	int i = x * w + y;
	int tresh = w / 2;
	int cpt = 0;
	for(; y < w; y++,i++)
		cpt += (isl[i] > 0) ? 1 : 0;
	return cpt >= tresh;
		
}
int * flood_debug(int i, int * dim ,int * isl, int * fld)
{
	int * q = malloc(sizeof(int));
	int * res = malloc(4*sizeof(int));
	res[0] = dim[1]/dim[0];
	res[1] = dim[0];
	res[2] = res[3] = 0;
	q[0] = i;
	int len = 1;
	int x,y,dn,up,l,r;
	while(len > 0)
	{
		len--;
		i = q[len];
		q = realloc(q,len*sizeof(int));
		isl[i] = 0;
		fld[i] = 0xFFFFFFFF;
		res[3]++;
		x = i / dim[0];
		y = i % dim[0];

		if(x <= res[0] && y < (dim[0]/3) && y < res[1])
//		&& grid_check(x,y,dim,isl))
		{
			res[0] = x;
			res[1] = y;
		}
		if(x == res[0] && y > res[2])
			res[2] = y;

		//4 coordinates to check
		dn = i - dim[0];
		up = i + dim[0];
		r = i + 1;
		l = i - 1;

		if(dn >= 0 && dn < dim[1] && isl[dn] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = dn;	
		}
		if(up >= 0 && up < dim[1] && isl[up] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = up;	
		}
		if(r >= 0 && r < dim[1] && isl[r] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = r;	
		}
		if(l >= 0 && l < dim[1] && isl[l] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = l;	
		}
	}
	free(q);
	return res;
}
int * flood_print(int i, int * dim ,int * isl, int color)
{
	int * q = malloc(sizeof(int));
	int * res = malloc(4*sizeof(int));
	res[0] = dim[1]/dim[0];
	res[1] = dim[0];
	res[2] = res[3] = 0;
	q[0] = i;
	int len = 1;
	int x,y,dn,up,l,r;
	while(len > 0)
	{
		len--;
		i = q[len];
		q = realloc(q,len*sizeof(int));
		isl[i] = color;
		res[3]++;
		x = i / dim[0];
		y = i % dim[0];

		if(x <= res[0] && y < res[1])
		{
			res[0] = x;
			res[1] = y;
		}
		if(x == res[0] && y > res[2])
			res[2] = y;

		//4 coordinates to check
		dn = i - dim[0];
		up = i + dim[0];
		r = i + 1;
		l = i - 1;

		if(dn >= 0 && dn < dim[1] && isl[dn] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = dn;
		}
		if(up >= 0 && up < dim[1] && isl[up] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = up;
		}
		if(r >= 0 && r < dim[1] && isl[r] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = r;
		}
		if(l >= 0 && l < dim[1] && isl[l] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = l;
		}
	}
	free(q);
	return res;
}
int * flood_fill(int i, int * dim ,int * isl)
{
	int * q = malloc(sizeof(int));
	int * res = malloc(4*sizeof(int));
	res[0] = dim[1]/dim[0];
	res[1] = dim[0];
	res[2] = res[3] = 0;
	q[0] = i;
	int len = 1;
	int x,y,dn,up,l,r;
	while(len > 0)
	{
		len--;
		i = q[len];
		q = realloc(q,len*sizeof(int));
		isl[i] = 0;
		res[3]++;
		x = i / dim[0];
		y = i % dim[0];

		if(x <= res[0] && y < res[1])
		{
			res[0] = x;
			res[1] = y;
		}
		if(x == res[0] && y > res[2])
			res[2] = y;

		//4 coordinates to check
		dn = i - dim[0];
		up = i + dim[0];
		r = i + 1;
		l = i - 1;

		if(dn >= 0 && dn < dim[1] && isl[dn] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = dn;	
		}
		if(up >= 0 && up < dim[1] && isl[up] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = up;	
		}
		if(r >= 0 && r < dim[1] && isl[r] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = r;	
		}
		if(l >= 0 && l < dim[1] && isl[l] > 0)
		{
			len++;
			q = realloc(q,len*sizeof(int));
			q[len-1] = l;	
		}
	}
	free(q);
	return res;
}
void print_grid(SDL_Surface * sco)
{
	int * pixels = sco->pixels;
	int w = sco->w;
	int h = sco->h;
	int len = w*h;
	int dim[2] = {w,len};
	int * isl = malloc(len*sizeof(int));
	int i;

	SDL_LockSurface(sco);

	//copy of pixel array
	for(i = 0;i < len; i++)
		isl[i] = pixels[i];

	int max = 0;
	//holds lowest coordinates : x1 y1 ; plus max y2 on line x1
	int * mem;
	int trash = -1;
	int color = -1;

	//performing flood-fill of black pixels
	for(i = 0; i < len; i++)
	{
		// pixel is white
		if(isl[i] > 0)
		{
			mem = flood_print(i,dim,isl,trash);
			if(mem[3] > max)
			{
				max = mem[3];
				color = trash;
			}
			--trash;
		}
	}

	for(i = 0; i < len; i++)
		pixels[i] = (isl[i] == color) ? 0xFFFFFFFF: 0;

	SDL_UnlockSurface(sco);
	free(isl);
}


void extract_grid(SDL_Surface ** sco)
{
	int * pixels = (*sco)->pixels;
	int w = (*sco)->w;
	int h = (*sco)->h;
	int len = w*h;
	int dim[2] = {w,len};
	int * isl = malloc(len*sizeof(int));
	int * fld = calloc(len,sizeof(int));
	int i;

	SDL_LockSurface(*sco);

	//copy of pixel array
	for(i = 0;i < len; i++)
		isl[i] = pixels[i];

	int max = 0;
	//holds lowest coordinates : x1 y1 ; plus max y2 on line x1
	int coo[3] = {0,0,0};
	int * mem;

	//performing flood-fill of black pixels
	for(i = 0; i < len; i++)
	{
		// pixel is white
		if(isl[i] > 0)
		{
			//mem = flood_fill(i,dim,isl);
			mem = flood_debug(i,dim,isl,fld);
			if(mem[3] > max)
			{
			//	for(int j = 0; j < len; j++)
			//		pixels[j] = fld[j];
				max = mem[3];
				coo[0] = mem[0];
				coo[1] = mem[1];
			       	coo[2] = mem[2]; 
			}
			//for(int j = 0; j < len; j++)
			//	fld[j] = 0;
		}
	}

	SDL_UnlockSurface(*sco);

	int side = coo[2] - coo[1];
//	side += 5;
//	coo[1] -= 10;
//	coo[0] += 40;
	
	//crop square grid from upper left corner
	printf("x:%d y:%d side :%d max: %d\n",coo[0],coo[1],side,max);
	const SDL_Rect src = {coo[1],coo[0],side,side};
	SDL_Rect dst = {0,0,side,side};

	SDL_Surface * grid = SDL_CreateRGBSurface(0,side,side,32,0,0,0,0);

	SDL_BlitSurface(*sco,&src,grid,&dst);

	free(isl);
	free(fld);

	IMG_SavePNG(grid,"grid.png");
	//IMG_SavePNG(sco,"grid.png");

	//Missing free the previous surface
	*sco = grid;
	//SDL_FreeSurface(grid);
}

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
#define MASK(A) (((A) & 0x0000FF00) >> 8)
void integral_image(SDL_Surface * surface, int * integral)
{
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	SDL_LockSurface(surface);

	integral[0] = MASK(pixels[0]);
	for(int i = 1; i < w; i++)
	{
		integral[i] = integral[i-1] + MASK(pixels[i]);
//		printf("%d ",integral[i]);
	}
//	printf("w:%d h:%d\n",w,h);
//	printf("%d\n",w*h);
	for(int j = 1; j < h; j++)
	{
		integral[j*w] = integral[(j-1)*w] + MASK(pixels[j*w]);
//		printf("%d ",integral[j]);
	}
	for(int i = 1; i < h; i++)
		for(int j = 1; j < w; j++)
		{
			int ind = i*w + j;
			int up = ind - w;
			int left = ind - 1;
			int ul = ind - w - 1;
			int val = MASK(pixels[ind]);
			integral[ind] = val + integral[up] + integral[left]
				- integral[ul];
//			printf("%d ",integral[ind]);
		}
//	printf("%d ",integral[w*h-1]);
	SDL_UnlockSurface(surface);
}
void surface_to_rotate_shear(SDL_Surface ** surface, const int theta)
{
	// rotates counterclockwise
	Uint32* pixels = (*surface)->pixels;
	int w = (*surface)->w;
	int h = (*surface)->h;
	int len = w * h;
	// dimensions of the bigger image
	int nd = (w > h) ? w : h;
	int nlen = nd * nd;
	SDL_Surface * new = SDL_CreateRGBSurface(0,nd,nd,32,0,0,0,0);
	Uint32* new_pixels = new->pixels;
	const double r_t = (double)theta * (PI/180.);
	double t = tan(r_t/2.);
	double s = sin(r_t);
	printf("rotated img by : %d°\n",theta);
	SDL_LockSurface(*surface);
	SDL_LockSurface(new);
	//unsigned char * bigger = calloc(len,1);//sizeof(char) = 1
	Uint32* bigger = calloc(nlen,sizeof(Uint32));
	for(int i = 0; i < len; i++)
	{
		int x = i / w;
		int y = i - (x * w);
		int d_x = x - h/2;
		int d_y = y - w/2;
		int nx = d_x - d_y*t;//first shear
		int ny = nx*s + d_y + w/2;//second shear
		nx = nx - (ny-w/2)*t + h/2; //third shear
		/*
		if(nx >= 0 && nx < h && ny >= 0 && ny < w)
		{	
			if(pixels[i] > 0)
				bigger[nx*w + ny] = pixels[i];//0xFF;
		}
		*/
		if(nx >= 0 && nx < nd && ny >= 0 && ny < nd)
		{	
			if(pixels[i] > 0)
				bigger[nx*w + ny] = pixels[i];//0xFF;
		}
	}
	//for(int i = 0; i < len; i++)
	for(int i = 0; i < nlen; i++)
	{
		/*
		Uint32 new = bigger[i];
		new = (new << 8) | bigger[i];
		new = (new << 8) | bigger[i];
		pixels[i] = (bigger[i] > 0) ? bigger[i] : 0;//0xFFFFFFFF : 0;
		*/
		new_pixels[i] = (bigger[i] > 0) ? bigger[i] : 0;//0xFFFFFFFF : 0;
	}
	SDL_UnlockSurface(*surface);
	SDL_UnlockSurface(new);
	*surface = new;
	//Free initial surface
	free(bigger);

	if(*surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
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
	if(st == 0)
		st = 0.001 ;
	//printf("Line : rho : %lf theta : %lf\n",r,ct);//acos(ct)*180/PI);
	SDL_LockSurface(surface);

	// x += 0.001 peut être diminué pour de l'opti
	for(x = 0; x < w; x+= 0.001)
	{
		y = (r/st)-x*(ct/st);
		X = (int)x;
		Y = (int)y;
		if( X >= 0 && X < w && Y >= 0 && Y < h)
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
void surface_to_hough(SDL_Surface * surface, int * min_theta)
{
	surface_to_canny(surface);

	Uint32* pixels = surface->pixels;
	const int w = surface->w;
	const int h = surface->h;
	//const int len = w * h;
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
	//printf("%d*%d  = %d ; %d \n",w,h,len,nlen);
	int max = 0;
	*min_theta = 180;
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
/*
   for(int h = -diag; h<diag; h++)
    {
        for(int w = 0; w<180; w++)
        {
            if(normalize(min, max, h_plane[h+diag][w]) >= threshold)
            {
                double cosT = cos(w * (M_PI / 180));
                double sinT = sin(w * (M_PI / 180)); 

                int x0 = h*cosT;
                int y0 = h*sinT;
                int x1 = x0 + diag * (-sinT);
                int y1 = y0 + diag * cosT;
                int x2 = x0 - diag * (-sinT);
                int y2 = y0 - diag * cosT; 

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                SDL_RenderDrawLine(renderer, x1,y1,x2,y2);
            }
        }
    }
*/
	int nbline = 0;
/*
	int * rhos = calloc(1000,sizeof(int));
        int * thetas = calloc(1000,sizeof(int));
        int * val = calloc(1000,sizeof(int));
*/
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
//((theta <= 100 && theta >= 80) || (theta >= -10 && theta <= 10))
				{
/*
					rhos[nbline-1] = i - diag;
					thetas[nbline-1] = theta;
					val[nbline-1] = v;
*/
					if(theta < *min_theta)
						*min_theta = theta;
					Draw_Polar_Line(surface,
					(double)(i-diag),coss[theta],sins[theta]);
				}
				delete_k_neigh(h_plane,diag/20,i,theta,a_r,sizeR);
			}
		}
	}

	printf("Detected line(s) : %d\n",nbline);
	printf("Angle of rotation detected : %d\n",*min_theta);
/*
	printf("Detected line(s) : %d\n",nbline);

	int ind = 0;
        for (int i = 0; i < 1 && nbline > 0; i++,nbline--)
        {
                ind = max_arr(val,1000);
                val[ind] = 0;
                Draw_Polar_Line(surface,
                        (double)rhos[ind],coss[thetas[ind]],sins[thetas[ind]]);
        }

	// h_plane holds the hough space; the accumulator
	free(val);
	free(rhos);
	free(thetas);
*/
	free(h_plane);
	free(coss);
	free(sins);
	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
void gray(SDL_Surface * sco)
{
	Uint32* pixels = sco->pixels;
	int w = sco->w;
	int h = sco->h;
	int len = w * h;
	SDL_PixelFormat* format = sco->format;
	SDL_LockSurface(sco);

	//computes grayscale
	for(int i = 0; i < len; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pixels[i],format, &r, &g, &b);
		float average = 0.3*r + 0.59*g + 0.11*b;
		Uint8 grays = (Uint8)average;
		//Transfer to new surface
		Uint32 new = grays;
		new = (new << 8) | grays;
		new = (new << 8) | grays;
		pixels[i] = new;
	}
	SDL_UnlockSurface(sco);
	if(sco == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
#define C(A,B) ((A) >= 0 && (A) < (B))
void average(SDL_Surface * sco, int size)
{
	Uint32* pixels = sco->pixels;
	int w = sco->w;
	int h = sco->h;
	int len = w * h;
	SDL_LockSurface(sco);
	
	//computes grayscale
	gray(sco);
	int * inte = calloc(len,4);
	integral_image(sco,inte);
	printf("%d\n",MASK(pixels[2000]));

	int rad = (size/2);
	// computes treshold based on neighbours 
	for(int i = 0; i < len; i++)
	{
		int x = i/w;
		int y = i%w;
		int ul = i - (rad)*w - rad - 1; //(rad+1)
		int ur = i - (rad)*w + rad; //(rad+1)
		int dl = i + rad*w - rad - 1;
		int dr = i + rad*w + rad;
		int x1 = x - (rad+1);
		int x2 = x + rad;
		int y1 = y - (rad+1);
		int y2 = y + rad;
		//Neutral value
		if(C(x1,h) && C(x2,h) && C(y1,w) && C(y2,w))
		{
			Uint32 sum = inte[ul] - inte[ur] - inte[dl] + inte[dr];
			Uint8 ave = sum/(size*size);
			Uint32 med = ave;
			med = (med << 8) | ave; 
			med = (med << 8) | ave; 
			pixels[i] = med;
		}
	}

	SDL_UnlockSurface(sco);
	if(sco == NULL)
		errx(EXIT_FAILURE,"%s", SDL_GetError());
}
int average3x3(Uint32 * neigh)
{
	int sum = 0;
	int pop_size = 9;
	for(int i = 0; i < 9; i++)
		sum += neigh[i];
	return sum / pop_size;

}
double std_dev(Uint32 * neigh, int mean)
{
	int pop_size = 9;
	double sum = 0;
	for(int i = 0; i < 9; i++)
	{
		double p = (double)(neigh[i] - mean);
		sum += p*p;
	}
	sum = ((sqrt((double)sum))/(double)pop_size);
	return sum;
}
void sauvola(SDL_Surface * sco)
{
	Uint32* pixels = sco->pixels;
	int w = sco->w;
	int h = sco->h;
	int len = w * h;
	SDL_LockSurface(sco);
	
	Uint32 * treshs = calloc(len,4);
	//computes grayscale
	gray(sco);

	for(int i = 1; i < h - 1; i++)
	{
		for(int j = 1; j < w - 1;j++)
		{
			int ul = i - w - 1;
			int u = i - w; 
			int ur = i - w + 1; 
			int dl = i + w - 1;
			int d = i + w;
			int dr = i + w + 1;
			int r = i + 1;
			int l = i - 1;
			Uint32 neigh[9] = {pixels[i],pixels[ul],pixels[ur],pixels[u],pixels[d],pixels[dl],pixels[dr],pixels[r],pixels[l]};
			for(int l=0; l < 9; l++)
				neigh[l] = MASK(neigh[l]);
			double avg = (double)average3x3(neigh);
			double sd = std_dev(neigh,avg); 
			double k = 0.5;
			double R = 128.0;
			treshs[i*w+j] = (int)(avg*(1.0+ k*((sd/R)-1)));
		}
	}	
	for(int i = 0; i < len; i++)
	{
		pixels[i] = (MASK(pixels[i]) >= treshs[i]) ? 0xFFFFFFFF : 0x0 ;
	}

	free(treshs);
	SDL_UnlockSurface(sco);
	if(sco == NULL)
		errx(EXIT_FAILURE,"%s", SDL_GetError());
	
}
void basic_bn(SDL_Surface * sco, Uint32 tresh)
{
	Uint32* pixels = sco->pixels;
	int w = sco->w;
	int h = sco->h;
	int len = w * h;
	SDL_LockSurface(sco);
	
	//computes grayscale
	gray(sco);
	
	for(int i = 0; i < len; i++)
	{
		pixels[i] = (MASK(pixels[i]) >= tresh) ? 0xFFFFFFFF : 0x0 ;
	}

	SDL_UnlockSurface(sco);
	if(sco == NULL)
		errx(EXIT_FAILURE,"%s", SDL_GetError());

	surface_to_invert(sco);
	surface_to_closing(sco);
	surface_to_dilatation(sco);
}
void speed_ada(SDL_Surface * sco, int size)
{
	Uint32* pixels = sco->pixels;
	int w = sco->w;
	int h = sco->h;
	int len = w * h;
	SDL_LockSurface(sco);
	
	//computes grayscale
	gray(sco);
	
	//average(sco,9);
	int * inte = calloc(len,4);
	integral_image(sco,inte);
	printf("%d\n",MASK(pixels[2000]));

	int rad = (size/2);
	// computes treshold based on neighbours 
	for(int i = 0; i < len; i++)
	{
		int x = i/w;
		int y = i%w;
		int ul = i - (rad)*w - rad - 1; //(rad+1)
		int ur = i - (rad)*w + rad; //(rad+1)
		int dl = i + rad*w - rad - 1;
		int dr = i + rad*w + rad;
		int x1 = x - (rad+1);
		int x2 = x + rad;
		int y1 = y - (rad+1);
		int y2 = y + rad;
		//Neutral value
		Uint32 tresh = 126;
		if(C(x1,h) && C(x2,h) && C(y1,w) && C(y2,w))
		{
			Uint32 sum = inte[ul] - inte[ur] - inte[dl] + inte[dr];
			tresh = sum/(size*size);
//			printf("%d ",tresh);
		}
		pixels[i] = (MASK(pixels[i]) >= tresh) ? 0xFFFFFFFF : 0x0 ;
	}

	SDL_UnlockSurface(sco);
	if(sco == NULL)
		errx(EXIT_FAILURE,"%s", SDL_GetError());

	surface_to_invert(sco);
	surface_to_closing(sco);
	surface_to_dilatation(sco);
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
				//further coordinates check is useless
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

	surface_to_invert(surface);
	surface_to_closing(surface);
	surface_to_dilatation(surface);
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
double convolution(Uint8* pixels, const int *mask, int len)
{
	double convol = 0;
	
	for(int i = 0; i < len; i++)
		convol += mask[i] * (double)pixels[i];

	return convol;
}
double pixel_to_blur(Uint8* pixels, int mask)
{

	double G, f;
	switch(mask)
	{
		case 5:
			G = convolution(pixels, Gaussian_by_five,mask*mask); 
			f = 1./273.; 
			break;
		case 7:
			G = convolution(pixels, Gaussian_by_seven, mask*mask);
			f = 1./1003.; 
			break;
		default:
			G = convolution(pixels, Gaussian_by_five, mask*mask);
			f = 1./273.; 
			break;
	}

	return f * G;
}
int pixel_to_angle(Uint8* pixels)
{
	double Gx = (1./9.) * convolution(pixels, sobel_Gx, MASK_LEN_SOBEL);
	double Gy = (1./9.) * convolution(pixels, sobel_Gy, MASK_LEN_SOBEL);

	if(Gx < 0)
		Gx *= (-1.);
	if(Gy < 0)
		Gy *= (-1.);
	double deg_convert = 180.000 / PI;
	double angle = atan2(Gy,Gx) * deg_convert;
	angle = (angle > 0) ? angle : angle + 90;

	// rounds the angle to four directions
	// 0 - 45 - 90 - 135
	double r_angle ;
/*
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
*/
	if(angle >= -22.500 && angle < 22.500)
		r_angle = 0;
	if(angle >= 157.500 && angle < 202.500)
		r_angle = 0;
	if(angle >= 22.500 && angle < 67.500)
		r_angle = 45;	
	if(angle >= 202.500 && angle < 247.500)
		r_angle = 45;	
	if(angle >= 67.500 && angle < 112.500)
		r_angle = 90;
	if(angle >= 247.5 && angle < 292.500)
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
void surface_to_blur(SDL_Surface * surface, int size)
{
	//kernel of size : (2*size)+1
	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
	int mask = 2*size+1;
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
		Uint8 neighb[mask*mask];
		int k = 0;
		for(int u = -size ; u < size+1 ; u++)
		{
			for(int bias = -size; bias < size+1; bias++)
			{
				int j = i + u * w + bias;
				if(j >= 0 && j < len)
					neighb[k] = grayscale[j];
				else
					neighb[k] = grayscale[i];
				k++;
			}
		}
		grayscale[i] = (unsigned int)(pixel_to_blur(neighb, mask));
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
	surface_to_blur(surface,KERNEL_SIZE);

	Uint32* pixels = surface->pixels;
	int w = surface->w;
	int h = surface->h;
	int len = w * h;
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
	int low_tresh = 20;
	int high_tresh = 80;
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
		if(sob[i] > 0)
			pixels[i] = 0xFFFFFFFF;
		else
			pixels[i] = 0;
	}

	free(sob);

	SDL_UnlockSurface(surface);
	if(surface == NULL)
        	errx(EXIT_FAILURE,"%s", SDL_GetError());
}
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
	long double sigma_t;
	long double sigma_t_max = BLACK;
	for(int i = 0; i < MAX_PIX_VAL; i++)
	{
		sigma_t = sigma_b_t(i,len,histo);
		sigma_t_max = (sigma_t > sigma_t_max) ? sigma_t : sigma_t_max;
	}
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
