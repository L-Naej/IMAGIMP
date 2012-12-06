#ifndef LUT_H
#define LUT_H

typedef enum lut_function{
	INVERT,
	ADDLUM,
	DIMLUM,
	ADDCON,
	DIMCON
	//SEPIA
} LUT_FUNCTION;

typedef struct lut{
	unsigned char* inputArrayRGB;
	unsigned char* outputArrayRGB;
	long int size;
	int maxValue;
	
	LUT_FUNCTION function;
}Lut;

Lut* createLut(unsigned char* inputArray, long int size, int maxVal, LUT_FUNCTION lF, unsigned char value);
void freeLut(Lut* lt);

#endif
