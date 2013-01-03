#include "lut.h"
#include "lutfunction.h"
#include <stdlib.h>

Channels* allocChannels(int size){
	int i =0;
	Channels* chan = (Channels*) calloc(1, sizeof(Channels));
	if(chan == NULL) return NULL;
	
	chan->chan1 = chan->chan2 = chan->chan3 = NULL;
	
	chan->chan1 = (unsigned char*) calloc(size, sizeof(unsigned char));	
	if(chan->chan1 == NULL) return NULL;
	for(i = 0; i < size; i++){
		chan->chan1[i] = i;
	}
	
	chan->chan2 = (unsigned char*) calloc(size, sizeof(unsigned char));	
	if(chan->chan2 == NULL) return NULL;
	for(i = 0; i < size; i++){
		chan->chan2[i] = i;
	}
	
	chan->chan3 = (unsigned char*) calloc(size, sizeof(unsigned char));	
	if(chan->chan3 == NULL) return NULL;
	for(i = 0; i < size; i++){
		chan->chan3[i] = i;
	}
	
	return chan;
}

/*
 * chan 1 = red; chan2 => green; chan3 = blue
 */
Channels* createChanFromRGB(unsigned char* arrayRGB, int size){
	if(arrayRGB == NULL) return NULL;
	int i = 0;
	Channels* chan = allocChannels(size / 3);
	if(chan == NULL) return NULL;
	
	for(i = 0; i < size / 3; i+=3){
		chan->chan1[i] = arrayRGB[i];
		chan->chan2[i] = arrayRGB[i+1];
		chan->chan3[i] = arrayRGB[i+2];
	}
	
	return chan;
}


/**
 * Créé une nouvelle LUT. 
 * @param inputArray: pixels sur lesquels appliquer la LUT
 * @param size: nombre de pixels
 * @param lF: fonction appliquée par la LUT 
 * @see enum lut_function
 */
Lut* createLut(Channels* input, LUT_FUNCTION lF, int val, int maxVal){
	if(input == NULL) return NULL;
	int i;
	Lut* lt = (Lut*) malloc(sizeof(Lut));
	if(lt==NULL) return NULL;
	
	/*
	lt->inputArrayRGB = NULL;
	lt->inputArrayR = NULL;
	lt->inputArrayG = NULL;
	lt->inputArrayB = NULL;
	lt->outputArrayRGB = NULL;
	lt->outputArrayR = NULL;
	lt->outputArrayG = NULL;
	lt->outputArrayB = NULL;
	*/
	lt->function = lF;
	lt->maxValue = maxVal;
	lt->size = maxVal+1;
	if(val < 0) val = 0;
	if(val > maxVal) val = maxVal;
	
	lt->valueEffect = val;
	
	if (lt->function!=SEPIA){
		/*
		lt->outputArrayRGB = (unsigned char*) calloc(lt->size, sizeof(unsigned char));
	
		if(lt->outputArrayRGB == NULL){
			free(lt);
			return NULL;
		}
		*/
		//Valeurs de 0 à maxValue => maxValue + 1 car il faut compter le 0
		lt->channels = allocChannels(lt->maxValue +1);
		
		switch(lt->function){
			case NEUTRAL : neutral(lt, input);
			break;
			case INVERT : invertLut(lt, input);
			break;
			case ADDLUM : addLum (lt, input);
			break;
			case DIMLUM : dimLum (lt, input);
			break;
			
			case ADDCON : addContrast (lt, input);
			break;
			case DIMCON : dimContrast (lt, input);
			break;
			default : fprintf(stderr,"Fonction LUT inconnue.\n");
			break;
		}
		
	return lt;
	}
	//SEPIA TODO
	/*
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
		return lt;
	}
	*/
	return NULL;
}

/**
 * Libère la mémoire allouée pour contenir une LUT.
 * @param lt le pointeur vers la zone mémoire à libérer.
 */
void freeLut(Lut* lt){
	if(lt == NULL) return;
	/*
	if(lt->inputArrayRGB)
		free(lt->inputArrayRGB);
	if(lt->inputArrayR)
		free(lt->inputArrayR);
	if(lt->inputArrayG)
		free(lt->inputArrayG);
	if(lt->inputArrayB)
		free(lt->inputArrayB);
	if(lt->outputArrayRGB)
		free(lt->outputArrayRGB);
	if(lt->outputArrayR)
		free(lt->outputArrayR);
	if(lt->outputArrayG)
		free(lt->outputArrayG);
	if(lt->outputArrayB)
		free(lt->outputArrayB);
	*/
	freeChannels(lt->channels);
	free(lt);
}
 
void applyLutToImg(Image* img,Lut* lut){
	int i;
	if (img==NULL || lut==NULL ) return;
	/*
	if (lut->function==SEPIA){
		for(i=0; i<(img->width)*(img->height)*3; i+=3){
			img->arrayRGB[i]=(unsigned char)(lut->outputArrayR[img->arrayRGB[i]]);
			img->arrayRGB[i+1]=(unsigned char)(lut->outputArrayG[img->arrayRGB[i+1]]);
			img->arrayRGB[i+2]=(unsigned char)(lut->outputArrayB[img->arrayRGB[i+2]]);
		}
	}
	*/
	//else{
		for(i=0; i<(img->width)*(img->height)*3; i+=3){
			img->arrayRGB[i] = lut->channels->chan1[img->arrayRGB[i]];
			img->arrayRGB[i+1] = lut->channels->chan2[img->arrayRGB[i+1]];
			img->arrayRGB[i+2] = lut->channels->chan3[img->arrayRGB[i+2]];
		}
	//}
}

void dumpLut(Lut* lt){
	if(lt == NULL) return;
	
	printf("\n----AFFICHAGE LUT-------\n");
	printf("Adresse : %p\n", lt);
	printf("Fonction : ");
	switch(lt->function){
			case NEUTRAL : printf("Neutre\n");
			break;
			case INVERT : printf("Inversion\n");
			break;
			case ADDLUM : printf("ADDLUM\n");
			break;
			case DIMLUM : printf("DIMLUM\n");
			break;
			case ADDCON : printf("ADDCON\n");
			break;
			case DIMCON : printf("DIMCON\n");
			break;
			default : fprintf(stderr,"Fonction LUT inconnue.\n");
			break;
	}
	printf("Valeur : %d\n", lt->valueEffect);
	//printf("OutputArray (adresse): %p\n", lt->outputArrayRGB);
	
}

void freeChannels(Channels* chan){
	free(chan->chan1);
	free(chan->chan2);
	free(chan->chan3);
	free(chan);
}
