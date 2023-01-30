//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


#include <SDL.h>

#ifdef EMSCRIPTEN
	#include <emscripten.h>
#else
	#include <stdio.h>
#endif

#include <stdbool.h>
#include <stdlib.h> // abs(), + others ?
#include <math.h> // trig
const double Tau = M_PI * 2;

#include "types/all_types.c"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))


#if 1

#define SCREEN_X 1024
#define SCREEN_Y 1024
#define SOURCE_X 256
#define SOURCE_Y 256

#else

#define SCREEN_X 512
#define SCREEN_Y 512
#define SOURCE_X 64
#define SOURCE_Y 64

#endif


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



Model model;


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
Uint8 * pixels;


int mouse_x, mouse_y;


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



// // Rotate around the origin. 
// // If you need to rotate around something else, please translate your point first.
// v2 RotatePoint(v2 point, f64 rotation)
// {

// 	// 1. Transform into { Angle, Length }.
// 	f64 Angle  = atan2(point.y, point.x); // radians
// 	f64 Length = sqrt(point.x * point.x + point.y * point.y);

// 	// 2. Ro-ta-te !
// 	Angle += rotation;

// 	// 3. Transform back into { X, Y }.

// 	// return { sin(Angle)*Length, cos(Angle)*Length };
// 	// alas .. maybe with Clang? TODO

// 	v2 newPoint = {
// 		.x = cos(Angle)*Length,
// 		.y = sin(Angle)*Length
// 	};

// 	return newPoint;
// }


f32 rotation_y = 0;
f32 rotation_x = 0;


void
DrawModel(Model m) {
	
	// copy vertex data so we can transform it
	int vert_count = m.vertices.count;
	int verts_size = vert_count * sizeof(Vertex);
	Vertex * verts = malloc(verts_size);
	memcpy(verts, m.vertices.data, verts_size);

	// rotate horizontally

	rotation_y = 0.5 + (float)mouse_x / SCREEN_X * Tau * 4; // not a typo, we rotate *around* y axis when mouse moves horizontally
		// Not sure why I need 8 Pi to achieve a single rotation though ...

	//rotation_y += 0.0015;

	//printf("rotation: %.1f", rotation);

	for ( int i = 0; i < vert_count; i++ ) {

		f64 Angle  = atan2(verts[i].z, verts[i].x); // radians
		f64 Length = sqrt(verts[i].x * verts[i].x + verts[i].z * verts[i].z);

		Angle += rotation_y;

		verts[i].x = cos(Angle)*Length;
		verts[i].z = sin(Angle)*Length;

	}

	// rotate vertically
	// rotation_x = rotation_x + 0.0013;
	rotation_x = (((float)mouse_y / SCREEN_Y) - 0.2) * Tau * 4; // not a typo, we rotate *around* x axis when mouse moves vertically

	//printf("rotation: %.1f", rotation);

	for ( int i = 0; i < vert_count; i++ ) {

		f64 Angle  = atan2(verts[i].z, verts[i].y); // radians
		f64 Length = sqrt(verts[i].y * verts[i].y + verts[i].z * verts[i].z);

		Angle += rotation_x;

		verts[i].y = cos(Angle)*Length;
		verts[i].z = sin(Angle)*Length;

	}

	// perspective 

	for (int i = 0; i < vert_count; i++) {

		verts[i].z += 2;

		verts[i].x /= verts[i].z;
		verts[i].y /= verts[i].z;
	}

	// transform into screen space

	for ( int i = 0; i < vert_count; i++ ) {

		// flip vertical because OBJ is upside down for some reason	
		verts[i].y *= -1;

		// scale from -1,1 to internal screen space
		verts[i].x *= SOURCE_X / 3;
		verts[i].y *= SOURCE_Y / 3;

		// center
		verts[i].x += SOURCE_X / 2; 
		verts[i].y += SOURCE_Y / 2;
	}

	// draw

	// printf("vert_count: %d\n", vert_count);

	for ( int i = 0; i < vert_count; i++ ) {
		int x = (int)CLAMP(verts[i].x, 0, SOURCE_X);
		int y = (int)CLAMP(verts[i].y, 0, SOURCE_X);  

		black(x,y);
		// printf("(%d,%d)\n",x,y);
	}

	// printf("\n");

	free(verts); // todo just pre-allocate a chunk of memory for this at init

}





Line line1 = {0, 0, SOURCE_X/2, SOURCE_X/2};


void
tick() {

	// update

	SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x = CLAMP(mouse_x/SCALE, 0, SCREEN_X);
	mouse_y = CLAMP(mouse_y/SCALE, 0, SCREEN_Y);

	// line1.x0 = mouse_x;
	// line1.y0 = mouse_y;

	// draw

	GfxBegin();
	Clear();

	// DrawLine(line1);
	// draw_rgba(10, 10, 255, 0, 255, 64);

	DrawModel(model);

	GfxRender();

	//_CrtDumpMemoryLeaks();
}

int main(int argc, char* argv[]) {


	printf("Hello world!\n");

	// PrintModel(model);

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_X, SCREEN_Y, 0, &window, &renderer);
	surface = SDL_CreateRGBSurface(0, SOURCE_X, SOURCE_Y, 32, 0, 0, 0, 0);


	///

	// model = LoadModel("assets/plane.obj");
	// model = LoadModel("assets/monkey.obj");
	//model = LoadModel("assets/monkey2.obj");
	 model = LoadModel("assets/monkey3.obj");


	///

	#ifdef EMSCRIPTEN 
		emscripten_set_main_loop(tick, 0, 1);
	#else
		for(;;) {
			tick();

			SDL_Event event;
			// SDL_WaitEvent(&event);
			while(SDL_PollEvent(&event)) {

				if (event.type == SDL_QUIT ) 
					return 0; // quit

				if (event.type == SDL_KEYDOWN)
					if (event.key.keysym.sym == SDLK_ESCAPE)
						return 0; // quit
			}
		}

	#endif

	return 0;
}