#include "image.c"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: image-file-test- OPTIONAL : degree");

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
    	SDL_Surface* sco;
	printf("Sample image : image_0%d.jpeg\n",argv[1][0]- '0');
    	switch(argv[1][0] - '0')
	{
		case 1:
    			sco = load_image("../imgs/image_01.jpeg");
			break;
		case 2:
    			sco = load_image("../imgs/image_02.jpeg");
			break;
		case 3:
    			sco = load_image("../imgs/image_03.jpeg");
			break;
		case 4:
    			sco = load_image("../imgs/image_04.jpeg");
			break;
		case 5:
    			sco = load_image("../imgs/image_05.jpeg");
			break;
		case 6:
    			sco = load_image("../imgs/image_06.jpeg");
			break;
		case 7:
    			sco = load_image("../imgs/rot_sudok.png");
			break;
		case 8:
    			sco = load_image("../imgs/lines.png");
			break;
		case 9:
    			sco = load_image("../imgs/canny_sudok.jpg");
			break;
		default:
    			sco = load_image(argv[1]);
			break;
	}
    // - Create a texture from the colored surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sco);

    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
/*
    w = sco->w;
    h = sco->h;
    len = w*h;
*/
    int angle = 0;
    int j = 1;
    if(argc > 3)
	    for(int i = 0; argv[3][i] != 0; i++, j *= 10)
		angle = angle * 10  + argv[3][i] - '0';
    int swich = (int)argv[2][0] - '0';
    printf("%s ","calling function");
    switch(swich)
    {
	    case 0:
		    if(argc > 3)
		    {
    		        printf("%s\n","Median filter");
	    		surface_to_median(sco);
		    }
		    else
		    {
    		        printf("%s\n","Gaussian blur");
	    		surface_to_blur(sco, KERNEL_SIZE);
		    }
		    break;
	    case 1:
		    if(argc > 3)
		    {
    			printf("%s\n","Otsu's binarisation");
	    		surface_to_BlackAndWhite(sco);
		    }
		    else
		    {
    			printf("%s\n","Adaptive binarisation");
			surface_to_adaptive_treshold(sco,1);
		    }
		break;
	    case 2:
    		printf("%s\n","Canny Edge");
	   	surface_to_canny(sco);
		IMG_SavePNG(sco,"canny_edge.png");
		break;
	    case 3:
    		printf("%s\n","Hough Transform");
	   	surface_to_canny(sco);
	    	surface_to_hough(sco,&angle);
		IMG_SavePNG(sco,"hough.png");
    	    	break;
	    case 4:
    		printf("%s %d degrees\n","Rotation from",angle);
	   	surface_to_canny(sco);
	    	surface_to_rotate_shear(sco, angle);
		IMG_SavePNG(sco,"rotate_shear.png");
		break;
	    case 5:
    		printf("%s \n","Grid detection");
	   	surface_to_canny(sco);
		//surface_to_grid(sco);
    	    	//surface_to_invert(sco);
	    	//surface_to_opening(sco);
	    	//surface_to_dilatation(sco);
	    	//surface_to_closing(sco);
	    	//surface_to_erosion(sco);
	        //RandomLines(sco,100);
		break;
	    case 6:
		// x1 x2 y1 y2 a b
		int* cood = line_inter(5.9,-6.98,5.6,-11.46,11.56,-13.54);
		printf("line intersection :%d %d\n",cood[0],cood[1]);
		break;
	    case 7:
		surface_to_adaptive_treshold(sco,10);
		surface_to_invert(sco);
		surface_to_closing(sco);
		surface_to_dilatation(sco);
		//surface_to_rotate_shear(sco, 22);
		extract_grid(sco);
		break;
	} 

    printf("%s\n","work done");

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, 1000, 1000);

    // - Create a new texture from the grayscale surface.
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
