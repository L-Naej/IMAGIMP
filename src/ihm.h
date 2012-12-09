#ifndef IHM_H
#define IHM_H
#include "layer.h"
#include "image.h"

Layer* parseCmdLine(int argc, char** argv);

int findNextLut(int argc, char** argv, int index, Lut** lt);

void displayImage(const Image* img);
#endif
