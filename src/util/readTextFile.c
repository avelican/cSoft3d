#ifndef READTEXTFILE_C
#define READTEXTFILE_C

// #include <stdio.h>

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

#include "../types/string.c"

String FileToString(char *fileName) {
	char * str;
	long len = ReadTextFile(fileName, &str);
	String res;
	res.length = len;
	res.string = str;
	return res;
}

#endif /* READTEXTFILE_C */