#include <SDL.h>
	#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // abs(), + others ?
#include <math.h> // trig
typedef struct Line {
	int x0;
	int y0;
	int x1;
	int y1;
} Line;
#include <stdlib.h> // EXIT_FAILURE
#include <stdint.h>
#include <stddef.h> // pointer types
#include <limits.h>
#include <float.h>
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef intptr_t intptr;
typedef uintptr_t uintptr;
// typedef size_t memory_index;
typedef float real32;
typedef double real64;
typedef int8 s8;
typedef int8 s08;
typedef int8 b8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;
typedef uint8 u8;
typedef uint8 u08;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;
typedef real32 r32;
typedef real64 r64;
typedef real32 f32;
typedef real64 f64;
typedef struct Vector3 {
	r32 x;
	r32 y;
	r32 z;
} Vector3;
typedef Vector3 Vertex;
// dynamic array
typedef struct VecVertex {
	int count; // todo probably make long
	int max_count;
	Vertex * data;
} VecVertex;
VecVertex MakeVecVertex() {
	VecVertex vec;
	vec.count = 0;
	vec.max_count = 1024;
	vec.data = malloc(vec.max_count * sizeof(Vertex));
	return vec;
}
void VecVertex_Grow(VecVertex * vec) {
	vec->max_count = vec->max_count * 2;
	int new_size = vec->max_count * sizeof(Vertex);
	Vertex * new_data = malloc(new_size);
	memcpy(new_data, vec->data, new_size);
}
void VecVertex_Push(VecVertex * vec, Vertex element) {
	if(vec->count == vec->max_count) {
		VecVertex_Grow(vec);
	}
	vec->data[vec->count] = element;
	vec->count++;
}
typedef struct Model {
	int vertex_count;
	VecVertex vertices;
} Model;
#include <stdio.h>
long ReadTextFile(char *fileName, char **_string) {
	int err = 0;
	FILE *f = fopen(fileName, "rb");
	fseek(f, 0, SEEK_END);
	const long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); // same as rewind(f);
	char *string = (char*) malloc(fsize + 1); // +1 for NULL byte
	*_string = string;
	fread(string, 1, fsize, f);
	fclose(f);
	string[fsize] = 0; // set NULL byte
	return fsize;
}
typedef struct String {
	int length;
	char * string;
} String;
String FileToString(char *fileName) {
	char * str;
	long len = ReadTextFile(fileName, &str);
	String res;
	res.length = len;
	res.string = str;
	return res;
}
Model LoadModel(char * filename) {
	// String model_str = FileToString(filename);
	Model model;
	model.vertex_count = 0; // ???
	model.vertices = MakeVecVertex();
	FILE* f = fopen(filename, "r");
	if(f == NULL) {
		perror("Error opening file.\n");
		// return NULL;
		return (Model) { 0 }; // VS shush
		exit(EXIT_FAILURE);
	}
	for(;;) {
		// read line
		char line[256];
		if ( fgets (line, 256, f) == NULL ) // EOF
			break;
		puts(line);
		// parse line
		char cmd[256];
		sscanf(line, "%s", cmd);
		if ( strcmp(cmd, "v") != 0)
			continue;
		Vertex vertex = {0};
		sscanf(line, "%s %f %f %f ", cmd, &vertex.x, &vertex.y, &vertex.z);
		// model.vertex_count++;
		VecVertex * verts = &model.vertices;
		VecVertex_Push(verts, vertex);
	}
	fclose(f);
  printf("Loaded model!\n");
  return model;
}
// Model ParseModel(String);
// Model LoadModel(char * filename) {
// 	String model_str = FileToString(filename);
// 	return ParseModel(model_str);
// }
// Model ParseModel(String model_str) {
// 	Model model;
// 	model.vertex_count = 0; // ???
// 	model.vertices = MakeVecVertex();
// 	int str_offset = 0;
// 	for(;;) {
// 		// read line
// 		char line[256];
// 		// int line_length;
// 		sscanf(model_str.string + str_offset, "%[^\n]s", line);
// 		printf("read line: %s\n", line);
// 		printf("str_offset: %d\n", str_offset);
// 		printf("line_length: %d\n\n", strlen(line));
// 		str_offset += strlen(line);
// 		// parse line
// 		char cmd[256];
// 		sscanf(line, "%s", cmd);
// 		if ( strcmp(cmd, "v") != 0) 
// 			continue;
// 		Vertex vertex;
// 		sscanf(line, "%s %f %f %f ", cmd, vertex.x, vertex.y, vertex.z);
// 		model.vertex_count++;
// 		VecVertex_Push(&model.vertices, vertex);
// 	}
// }
void PrintModel(Model model) {
  printf("Printing model!\n");
	for(int i = 0; i < model.vertices.count; i++) {
		printf( "v %.2f %.2f %.2f\n",
				model.vertices.data[i].x,
				model.vertices.data[i].y,
				model.vertices.data[i].z );
	}
  printf("Done printing model!\n");
}
#include <stdint.h>
#include <stddef.h> // pointer types
#include <limits.h>
#include <float.h>
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef intptr_t intptr;
typedef uintptr_t uintptr;
// typedef size_t memory_index;
typedef float real32;
typedef double real64;
typedef int8 s8;
typedef int8 s08;
typedef int8 b8;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;
typedef uint8 u8;
typedef uint8 u08;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;
typedef real32 r32;
typedef real64 r64;
typedef real32 f32;
typedef real64 f64;
typedef struct Vector2 {
	r32 x;
	r32 y;
} Vector2;
const int SCALE = 512 / 64;
const int PIXEL_COUNT = 64 * 64 * 4;
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
	int dx = abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */
	for (;;){
		do { int offset = (y0 * 64 + x0) * 4; pixels[offset] = 0; pixels[offset+1] = 0; pixels[offset+2] = 0; pixels[offset+3] = 255; } while (0);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}
Line line1 = {0, 0, 64/2, 64/2};
void tick() {
	// update
	int mouse_x = 0;
	int mouse_y = 0;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_x = ((((512)<((((mouse_x/SCALE)>(0))?(mouse_x/SCALE):(0))))?(512):((((mouse_x/SCALE)>(0))?(mouse_x/SCALE):(0)))));
	mouse_y = ((((512)<((((mouse_y/SCALE)>(0))?(mouse_y/SCALE):(0))))?(512):((((mouse_y/SCALE)>(0))?(mouse_y/SCALE):(0)))));
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
	do { int offset = (10 * 64 + 10) * 4; pixels[offset] = 255; pixels[offset+1] = 0; pixels[offset+2] = 255; pixels[offset+3] = 64; } while (0);
	GfxRender();
}
int main(int argc, char* argv[]) {
	printf("Hello world!\n");
	Model model = LoadModel("assets/plane.obj");
	PrintModel(model);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(512, 512, 0, &window, &renderer);
	surface = SDL_CreateRGBSurface(0, 64, 64, 32, 0, 0, 0, 0);
		for(;;) {
			tick();
			SDL_Event event;
			SDL_WaitEvent(&event);
			if (event.type == SDL_QUIT )
				return;
		}
	return 0;
}
