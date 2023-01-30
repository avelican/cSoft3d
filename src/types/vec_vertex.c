// dynamic array
#ifndef VEC_VERTEX_C
#define VEC_VERTEX_C

#include "vertex.c"

typedef struct VecVertex {
	int count; // todo probably make long
	int max_count;
	Vertex * data;
} VecVertex;

#define STARTING_SIZE 1024000

VecVertex MakeVecVertex() {
	VecVertex vec;
	vec.count = 0;
	vec.max_count = STARTING_SIZE;
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

#endif /* VEC_VERTEX_C */