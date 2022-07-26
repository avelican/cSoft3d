// dynamic array
#ifndef VEC_ABSTRACT_C
#define VEC_ABSTRACT_C

// abstract type. do not instantiate

typedef int Abstract; // this file should not compile (should throw an error if you try to compile it!), so this is unnecessary.

typedef struct VecAbstract {
	int count; // todo probably make long
	int max_count;
	Abstract * data;
} VecAbstract;

#define STARTING_SIZE 1024

VecAbstract MakeVecAbstract() {
	VecAbstract vec;
	vec.count = 0;
	vec.max_count = STARTING_SIZE;
	vec.data = malloc(vec.max_count * sizeof(Abstract));
	return vec;
}

void VecAbstract_Grow(VecAbstract * vec) {
	vec->max_count = vec->max_count * 2;
	int new_size = vec->max_count * sizeof(Abstract);
	Abstract * new_data = malloc(new_size);
	memcpy(new_data, vec->data, new_size);
}

void VecAbstract_Push(VecAbstract * vec, Abstract element) {
	if(vec->count == vec->max_count) {
		VecAbstract_Grow(vec);
	}
	vec->data[vec->count] = element;
	vec->count++;
}

#endif /* VEC_ABSTRACT_C */