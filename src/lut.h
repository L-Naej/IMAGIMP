#ifndef LUT_H
#define LUT_H
#include "image.h"
typedef enum lut_function{
	INVERT,
	ADDLUM,
	DIMLUM,
	ADDCON,
	DIMCON,
	SEPIA,
	NEUTRAL
} LUT_FUNCTION;

typedef struct lut{
	unsigned char* inputArrayRGB;
	unsigned char* inputArrayR;
	unsigned char* inputArrayG;
	unsigned char* inputArrayB;
	unsigned char* outputArrayRGB;
	unsigned char* outputArrayR;
	unsigned char* outputArrayG;
	unsigned char* outputArrayB;
	long int size;
	int maxValue;
	
	LUT_FUNCTION function;
}Lut;

//Lut* createLut(unsigned char* inputArray, long int size, int maxVal, LUT_FUNCTION lF, int val);
Lut* createLut(LUT_FUNCTION lF, int val);
void freeLut(Lut* lt);
void applyLut(Image* img,LUT_FUNCTION lF, int val);

#endif
