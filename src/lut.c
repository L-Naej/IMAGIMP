#include "lut.h"
#include "lutfunction.h"
#include <stdlib.h>
#include <math.h>

Channels* allocChannels(int size){
	int i =0;
	Channels* chan = (Channels*) calloc(1, sizeof(Channels));
	if(chan == NULL) return NULL;
	
	chan->chan1 = chan->chan2 = chan->chan3 = NULL;
	
	chan->chan1 = (unsigned char*) calloc(size, sizeof(unsigned char));	
	if(chan->chan1 == NULL) return NULL;
	
	chan->chan2 = (unsigned char*) calloc(size, sizeof(unsigned char));	
	if(chan->chan2 == NULL){
		free(chan->chan1);
		return NULL;
	}
	
	chan->chan3 = (unsigned char*) calloc(size, sizeof(unsigned char));	
	if(chan->chan3 == NULL){
		free(chan->chan1);
		free(chan->chan2);
		return NULL;
	}
	
	for(i = 0; i < size; i++){
		chan->chan1[i] = i;
		chan->chan2[i] = i;
		chan->chan3[i] = i;
	}
	 
	chan->size = size;
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
	
	lt->function = lF;
	lt->maxValue = maxVal;
	lt->size = maxVal+1;
	if(val < 0) val = 0;
	if(val > maxVal) val = maxVal;
	
	lt->valueEffect = val;
	
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
		
		case ADDCON : changeContrast (lt, input);
		break;
		case DIMCON : 
			//On prend une valeur négative
			lt->valueEffect = - lt->valueEffect;
			changeContrast (lt, input);
		break;
		//Si cette fonction est passée par erreur, on renvoie NULL
		//Pour sépia appeler la fonction prévue pour
		case SEPIA : 
			freeLut(lt);
			lt = NULL;
		break;
		default : fprintf(stderr,"Fonction LUT inconnue.\n");
		break;
	}
	
return lt;

}

Lut* createSepiaLut(Image* imgSource){
	if(imgSource == NULL || imgSource->arrayRGB == NULL)
		return NULL;
	
	//Conversion de l'image en niveau de gris
	int i, result;
	unsigned char  r, g, b;
	
	for(i = 0; i < imgSource->width*imgSource->height*3; i+=3){
		r = imgSource->arrayRGB[i];
		g = imgSource->arrayRGB[i+1];
		b = imgSource->arrayRGB[i+2]; 
		//Méthode luminosity average pour calcul du niveau de gris
		result =  (int) floor(0.21*r + 0.71*g + 0.07*b);
		
		if(result > imgSource->maxValue) result = imgSource->maxValue;
		
		imgSource->arrayRGB[i] = (unsigned char) result;
		imgSource->arrayRGB[i+1] = (unsigned char) result;
		imgSource->arrayRGB[i+2] = (unsigned char) result;
	}
	
	//Création du lut sépia
	Lut* sepia = (Lut*) calloc(1,sizeof(Lut));
	if(sepia == NULL) return NULL;
	
	sepia->maxValue = imgSource->maxValue;
	sepia->size = imgSource->maxValue + 1;
	sepia->function = SEPIA;
	sepia->valueEffect = 0;
	sepia->channels = allocChannels(sepia->size);
	if(sepia->channels == NULL){
		free(sepia);
		return NULL;
	}
	
	//Remplissage de ses channels pour faire du sépia
	for(i = 0; i < sepia->size; ++i){
		result = sepia->channels->chan1[i] + 100;
		result = result > sepia->maxValue ? sepia->maxValue : result;
		result = result < 0 ? 0 : result;
		sepia->channels->chan1[i] = result;
		
		result = sepia->channels->chan2[i] + 50;
		result = result > sepia->maxValue ? sepia->maxValue : result;
		result = result < 0 ? 0 : result;
		sepia->channels->chan2[i] = result;
	}
	
	return sepia;
}

void regenerateLut(Lut** lt, Channels* input, Image* img){
	if(lt == NULL || *lt == NULL)
		return;
	
	switch((*lt)->function){
		case NEUTRAL : neutral(*lt, input);
		break;
		case INVERT : invertLut(*lt, input);
		break;
		case ADDLUM : addLum (*lt, input);
		break;
		case DIMLUM : dimLum (*lt, input);
		break;
		
		case ADDCON : changeContrast (*lt, input);
		break;
		case DIMCON : 
			//On prend une valeur négative
			(*lt)->valueEffect = - (*lt)->valueEffect;
			changeContrast (*lt, input);
		break;
		case SEPIA : 
			if(img == NULL) return;
			freeLut(*lt);
			*lt = createSepiaLut(img);
		break;
		default : fprintf(stderr,"Fonction LUT inconnue.\n");
		break;
	}
}

/**
 * Libère la mémoire allouée pour contenir une LUT.
 * @param lt le pointeur vers la zone mémoire à libérer.
 */
void freeLut(Lut* lt){
	if(lt == NULL) return;
	freeChannels(lt->channels);
	free(lt);
}
 
void applyLutToImg(Image* imgSource, Image* imgFinale, Lut* lut){
	int i;
	if (imgSource==NULL || imgFinale == NULL || lut==NULL ) return;
	
	for(i=0; i<(imgSource->width)*(imgSource->height)*3; i+=3){
		imgFinale->arrayRGB[i] = lut->channels->chan1[imgSource->arrayRGB[i]];
		imgFinale->arrayRGB[i+1] = lut->channels->chan2[imgSource->arrayRGB[i+1]];
		imgFinale->arrayRGB[i+2] = lut->channels->chan3[imgSource->arrayRGB[i+2]];
	}
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
			case SEPIA : printf("SEPIA\n");
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
