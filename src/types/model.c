#ifndef MODEL_C
#define MODEL_C
// #include <stdlib.h> // EXIT_FAILURE
#include "vertex.c"
#include "vec_vertex.c"

typedef struct Model {
	int vertex_count;
	VecVertex vertices;
} Model;

#include "../util/readTextFile.c"



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
	for(int i = 0; i < model.vertices.count; i++) {
		printf(	"v %.2f %.2f %.2f", 
				model.vertices.data[i].x,
				model.vertices.data[i].y,
				model.vertices.data[i].z );
	}
}

#endif /* MODEL_C */