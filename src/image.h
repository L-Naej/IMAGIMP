#ifndef IMAGE_H
#define IMAGE_H
#include "utils.h"
typedef struct image{
	short format;//P1 to P6
	char* comments;
	char* name;
	int width;
	int height;
	int maxValue;
	unsigned char* arrayRVB;
	
}Image;

Image* createEmptyImg(int w, int h);
Image* loadImage(char* fileName);
bool saveImage(Image* img);
Image* createEmptyImg();
void detectWH(const char* text, int* w, int* h);
void freeImage(Image* img);
bool imgAddName(Image* img, const char name[]);

#endif
