#include "lutfunction.h"
#include "lut.h"
#include <stdlib.h>

void invertLut(Lut* lt){
	int i;
	
	if(lt == NULL) return;
	
	for(i = 0; i < lt->size; ++i){
		lt->outputArrayRVB[i] = lt->maxValue - lt->inputArrayRVB[i];
	}
}
