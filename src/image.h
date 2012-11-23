#ifndef IMAGE_H
#define IMAGE_H
#include "utils.h"
typedef struct image Image;

Image* createEmptyImg(int w, int h);
Image* loadImage(char* fileName);
bool saveImage(Image* img);
void detectWH(const char* text, int* w, int* h);
void freeImage(Image* img);
bool imgAddName(Image* img, const char name[]);

#endif
