#ifndef IMAGE_H
#define IMAGE_H
#include "utils.h"
typedef struct image Image;

Image* loadImage(char* fileName);
bool saveImage(Image* img);
Image* createEmptyImg();
void detectWH(const char* text, int* w, int* h);
void freeImage(Image* img);

#endif
