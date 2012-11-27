#include "lut.h"
#include "lutfunction.h"
#include <stdlib.h>

/**
 * Créé une nouvelle LUT. 
 * @param inputArray: pixels sur lesquels appliquer la LUT
 * @param size: nombre de pixels
 * @param lF: fonction appliquée par la LUT 
 * @see enum lut_function
 */
Lut* createLut(unsigned char* inputArray, long int size, int maxVal, LUT_FUNCTION lF){
	if(inputArray == NULL) return NULL;
	
	Lut* lt = (Lut*) malloc(sizeof(Lut));
	lt->inputArrayRVB = inputArray;
	
	lt->function = lF;
	lt->maxValue = maxVal;
	lt->size = size;
	
	lt->outputArrayRVB = (unsigned char*) calloc(size, sizeof(unsigned char));
	
	if(lt->outputArrayRVB == NULL){
		free(lt);
		return NULL;
	}
	
	switch(lF){
		case INVERT : invertLut(lt);
		break;
		//Ajouter les autres
	}
	
	return lt;
}

/**
 * Libère la mémoire allouée pour contenir une LUT.
 * @param lt le pointeur vers la zone mémoire à libérer.
 */
void freeLut(Lut* lt){
	if(lt == NULL) return;
	
	free(lt->outputArrayRVB);
	free(lt);
}


