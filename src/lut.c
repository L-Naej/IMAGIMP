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
Lut* createLut(LUT_FUNCTION lF, int val){
	int i;
	Lut* lt = (Lut*) malloc(sizeof(Lut));
	lt->function = lF;
	lt->maxValue = 255;
	lt->size = 256;
	
	if (lt->function!=SEPIA){
		unsigned char* inputArray = NULL;
		inputArray=(unsigned char*) malloc((lt->maxValue)*sizeof(unsigned char));
		if(inputArray == NULL) return NULL;
		
		for(i=0;i<lt->size;i++){
			inputArray[i]=i;
			}
	
		lt->inputArrayRGB = inputArray;
		lt->outputArrayRGB = (unsigned char*) calloc(lt->size, sizeof(unsigned char));
	
		if(lt->outputArrayRGB == NULL){
			free(lt->inputArrayRGB);
			free(lt);
			return NULL;
		}
	}
	else {
		unsigned char* inputArrayR = NULL,*inputArrayG = NULL,*inputArrayB = NULL;
		inputArrayR=(unsigned char*) malloc((lt->maxValue)*sizeof(unsigned char));
		inputArrayG=(unsigned char*) malloc((lt->maxValue)*sizeof(unsigned char));
		inputArrayB=(unsigned char*) malloc((lt->maxValue)*sizeof(unsigned char));
	
		if(inputArrayR==NULL ||inputArrayG==NULL || inputArrayB==NULL){
			free(lt->outputArrayRGB);
			free(lt->inputArrayRGB);
			free(inputArrayB); free(inputArrayG); free(inputArrayR);
			free(lt);
			return NULL;	
		} 
		
		for(i=0;i<lt->size;i++){
			inputArrayR[i]=i;
			inputArrayG[i]=i;
			inputArrayB[i]=i;
			}
	
		lt->inputArrayR = inputArrayR;
		lt->inputArrayG = inputArrayG;
		lt->inputArrayB = inputArrayB;
		
		lt->outputArrayR = (unsigned char*) calloc(lt->size, sizeof(unsigned char));
		lt->outputArrayG = (unsigned char*) calloc(lt->size, sizeof(unsigned char));
		lt->outputArrayB = (unsigned char*) calloc(lt->size, sizeof(unsigned char));
		
		if(lt->outputArrayR == NULL || lt->outputArrayG == NULL || lt->outputArrayB == NULL){
			free(lt->outputArrayRGB);
			free(lt->inputArrayRGB);
			free(lt->inputArrayB); free(lt->inputArrayG); free(lt->inputArrayR);
			free(lt->outputArrayB); free(lt->outputArrayG); free(lt->outputArrayR);
			free(lt);
			return NULL;
		
		}
	}
		
	switch(lF){
		case INVERT : invertLut(lt);
		break;
		case ADDLUM : addLum (lt,val);
		break;
		case DIMLUM : dimLum (lt,val);
		break;
		case ADDCON : addContrast (lt,val);
		break;
		case DIMCON : dimContrast (lt,val);
		break;
		case SEPIA : sepia (lt);
		break;
	}
	
	return lt;
}

/**
 * Libère la mémoire allouée pour contenir une LUT.
 * @param lt le pointeur vers la zone mémoire à libérer.
 */
void freeLut(Lut* lt){
	if(lt == NULL) return;
	
	free(lt->outputArrayRGB);
	free(lt);
}


void applyLut(Image* img,LUT_FUNCTION lF, int val){
	int i;
	if (img==NULL) return;
	Lut* lut = createLut(lF,val);
	if (lut==NULL) return;
	
	if (lut->function==SEPIA){
		for(i=0; i<(img->width)*(img->height)*3; i+=3){
			img->arrayRGB[i]=(unsigned char)(lut->outputArrayR[img->arrayRGB[i]]);
			img->arrayRGB[i+1]=(unsigned char)(lut->outputArrayG[img->arrayRGB[i+1]]);
			img->arrayRGB[i+2]=(unsigned char)(lut->outputArrayB[img->arrayRGB[i+2]]);
			}
	}
	else{
		for(i=0; i<(img->width)*(img->height)*3; i+=3){
			img->arrayRGB[i]=(unsigned char)(lut->outputArrayRGB[img->arrayRGB[i]]);
			img->arrayRGB[i+1]=(unsigned char)(lut->outputArrayRGB[img->arrayRGB[i+1]]);
			img->arrayRGB[i+2]=(unsigned char)(lut->outputArrayRGB[img->arrayRGB[i+2]]);
			}
		}
}
