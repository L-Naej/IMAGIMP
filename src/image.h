#ifndef IMAGE_H
#define IMAGE_H

typedef struct image Image;

Image* loadImage(char* fileName);
int saveImage(Image*);
void detectWH(const char* text, int* w, int* h);
void freeImage(Image* img);

#endif
