#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
typedef enum {false=0, true=1} bool;  

bool readNUchar(unsigned char*, int n, FILE* f);
bool writeNUchar(unsigned char*, int n, FILE* f);
void invertPPMArray(unsigned char array[], long int width, long int height);
#endif
