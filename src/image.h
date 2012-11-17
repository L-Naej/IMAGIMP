#ifndef IMAGE_H
#define IMAGE_H

typedef struct image Image;

Image* loadImage(char* fileName);
int saveImage(Image*);

#endif
