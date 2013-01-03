#ifndef LUT_H
#define LUT_H
#include "image.h"
typedef enum lut_function{
	INVERT = 1,
	ADDLUM = 2,
	DIMLUM = 3,
	ADDCON = 4,
	DIMCON = 5,
	SEPIA = 6,
	NEUTRAL = 0
	} LUT_FUNCTION;

typedef struct {
	unsigned char* chan1;
	unsigned char* chan2;
	unsigned char* chan3;
} Channels;

typedef struct lut{
	/*
	unsigned char* inputArrayRGB;
	unsigned char* inputArrayR;
	unsigned char* inputArrayG;
	unsigned char* inputArrayB;
	unsigned char* outputArrayRGB;
	unsigned char* outputArrayR;
	unsigned char* outputArrayG;
	unsigned char* outputArrayB;
	*/
	Channels* channels;
	long int size;
	int maxValue;
	LUT_FUNCTION function;
	int valueEffect;
}Lut;



//Lut* createLut(unsigned char* inputArray, long int size, int maxVal, LUT_FUNCTION lF, int val);
Lut* createLut(Channels* input, LUT_FUNCTION lF, int val, int maxVal);

void freeLut(Lut* lt);

void applyLutToImg(Image* imgSource, Image* imgFinale, Lut* lut);

void dumpLut(Lut* lt);

Channels* allocChannels(int size);
Channels* createChanFromRGB(unsigned char* arrayRGB, int size);
void freeChannels(Channels* chan);
#endif
