#ifndef LUT_FUNC_H
#define LUT_FUNC_H
#include "lut.h"
void neutral(Lut* lt, Channels* input);

void invertLut(Lut* lt, Channels* input);

void addLum(Lut* lt, Channels* input);

void dimLum(Lut* lt, Channels* input);

void changeContrast(Lut* lt, Channels* input);

void sepia(Lut* lt, Channels* input);

Channels* rgbToYUV(Channels* input, Channels* output, int size, int maxValue);
Channels* yuvToRGB(Channels* input, Channels* output, int size, int maxValue);
#endif
