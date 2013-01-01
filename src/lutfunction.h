#ifndef LUT_FUNC_H
#define LUT_FUNC_H
#include "lut.h"
void neutral(Lut* lt);
void invertLut(Lut* lt);
void addLum(Lut* lt, int val);
void dimLum(Lut* lt, int val);
void addContrast(Lut* lt, int val);
void dimContrast(Lut* lt, int val);
void sepia(Lut* lt);

#endif
