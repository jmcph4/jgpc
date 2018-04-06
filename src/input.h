#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INIT_BUF_LEN 128
#define BUF_GROWTH_FACTOR 2

int grabeof(char** buf, FILE* fp);
int grabuntil(char** buf, bool* eof, char* delims, size_t num_delims,
        FILE* fp);
int grabline(char** buf, bool* eof, FILE* fp);
int readfile(const char* path, char** data, size_t* size);

#endif /* INPUT_H_ */

