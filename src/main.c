//#include <SDL.h>

#ifdef EMSCRIPTEN
	#include <emscripten.h>
#else
	// #include <stdio.h>
#endif

// #include <stdbool.h>
// #include <stdlib.h> // abs(), + others ?
// #include <math.h> // trig

#include "types/all_types.c"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))



#define SCREEN_X 512
#define SCREEN_Y 512
#define SOURCE_X 64
#define SOURCE_Y 64
const int SCALE = SCREEN_X / SOURCE_X;

#define COLOR_BYTES 4

const int PIXEL_COUNT = SOURCE_X * SOURCE_Y * COLOR_BYTES;



#define draw_rgba(x, y, r, g, b, a) do { \
	int offset = (y * SOURCE_X + x) * COLOR_BYTES; \
	pixels[offset]   = r; \
	pixels[offset+1] = g; \
	pixels[offset+2] = b; \
	pixels[offset+3] = a; \
} while (0)

#define draw_rgb(x, y, r, g, b) draw_rgba(x, y, r, g, b, 255)

#define draw_bw(x, y, c) draw_rgb(x, y, c, c, c)


#define black(x, y) draw_bw(x, y, 0)
#define white(x, y) draw_bw(x, y, 255)





SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;

Uint8 * pixels;


void Clear() {
		for (int i=0; i < PIXEL_COUNT; i++) {
		pixels[i] = 255;
	}
}

void GfxBegin() {
	if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

	pixels = surface->pixels;
}

void GfxRender() {
	if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	SDL_Texture *screenTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(screenTexture);
}

void BackGroundNoise() {
	
	for (int i=0; i < PIXEL_COUNT; i++) {
		// char randomByte = rand() % 255;
		char randomByte = 128 + rand() % 16;
		pixels[i] = randomByte;
	}
}



void
// line (int x0, int y0, int x1, int y1)
DrawLine (Line line)
{
	int x0 = line.x0;
	int y0 = line.y0;
	int x1 = line.x1;
	int y1 = line.y1;

	// https://gist.github.com/bert/1085538
	int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
	int err = dx + dy, e2; /* error value e_xy */
 
	for (;;){
		black(x0,y0);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}








Line line1 = {0, 0, SOURCE_X/2, SOURCE_X/2};





void tick() {

	// update

	int mouse_x = 0;
	int mouse_y = 0;

	SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x = CLAMP(mouse_x/SCALE, 0, SCREEN_X);
	mouse_y = CLAMP(mouse_y/SCALE, 0, SCREEN_Y);

	// printf("%d %d\n", mouse_x, mouse_y);

	line1.x0 = mouse_x;
	line1.y0 = mouse_y;


	// draw

	GfxBegin();

	Clear();
	// BackGroundNoise();
	// DrawLine(10, 20, 40, 50);


	// DrawLineAA_Bork(line1.x0, line1.y0, line1.x1, line1.y1);

	DrawLine(line1);

	draw_rgba(10, 10, 255, 0, 255, 64);

	GfxRender();
}




int main(int argc, char* argv[]) {


	printf("Hello world!\n");

	Model model = LoadModel("assets/plane.obj");
	PrintModel(model);

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_X, SCREEN_Y, 0, &window, &renderer);
	surface = SDL_CreateRGBSurface(0, SOURCE_X, SOURCE_Y, 32, 0, 0, 0, 0);


	#ifdef EMSCRIPTEN 
		emscripten_set_main_loop(tick, 0, 1);
	#else
		for(;;) {
			tick();

			SDL_Event event;
			SDL_WaitEvent(&event);
			if (event.type == SDL_QUIT ) 
				return;
		}
	#endif
	return 0;
}