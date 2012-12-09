#include "ihm.h"
#include <ctype.h>
#include <stdlib.h>
#include <interface.h>
#include <string.h>
#include "layer.h"

Layer* parseCmdLine(int argc, char** argv){
	if(argv == NULL) return false;
	
	if(argc < 2){
		fprintf(stderr, "Utilisation du programme : imagimp monimage.ppm [<code_lut>[_<param1>]*]*\n");
		return false;
	}
	
	Image* img = loadImage(argv[1]);
	int index = 0;
	Lut* currentLut = NULL;
	
	if(img == NULL){
		return NULL;
	}
	
	Layer* firstLay = createLayer(img, 1.0, SUM);//2 derniers params sans importance
	
	if(firstLay == NULL){
		freeImage(img);
		return NULL;
	}
	
	//Si pas de code LUT
	if(argc < 3) return firstLay;
	
	index = 2;
	
	index = findNextLut(argc, argv, index, &currentLut);
	do{
		if(currentLut)
			addLut(firstLay, currentLut);
	}while(index < argc);
	
	return firstLay;
}

int findNextLut(int argc, char** argv, int index, Lut** currentLut){
	LUT_FUNCTION ltFunc = NEUTRAL;
	int lutFuncValue = 0;
	
	//Vaudra true si la ligne de commande est mal formatée
	bool parseError = false;
	
	if(strcmp("ADDLUM", argv[index]) == 0){
		ltFunc = ADDLUM;
		//S'il existe encore un argument et que c'est un nombre
		//il s'agit de la valeur du lut
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (ADDLUM a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("DIMLUM", argv[index]) == 0) {
		ltFunc = DIMLUM;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (DIMLUM a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("ADDCON", argv[index]) == 0){
		ltFunc = ADDCON;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (ADDCON a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("DIMCON", argv[index]) == 0){
		ltFunc = DIMCON;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (DIMCON a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("INVERT", argv[index]) == 0){
		ltFunc = INVERT;
	}
	else if(strcmp("SEPIA", argv[index]) == 0){
		ltFunc = SEPIA;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (SEPIA a besoin d'une valeur)
		else parseError = true;
	}
	else{ 
		fprintf(stderr,"Code lUT inconnu : %s.\n", argv[index]);
		parseError = true;
	}
	
	
	
	if(!parseError){
		*currentLut = createLut(ltFunc, lutFuncValue);
	}
	else{
		*currentLut = NULL;
		printf("Erreur dans la ligne de commande autour du mot n° %d\n", index);
	}
	
	index++;
	return index;
}

void displayImage(const Image* img){
	if(img == NULL || img->arrayRGB == NULL) return;
	
	initGLIMAGIMP(img->width,img->height,img->arrayRGB);
	
}


void printState(){
	//Layer n°X
	//Image source image.ppm
	//LUT lut param1...(chaîne des lut)
	
	//Chaine des layers
}
void printMenu(){
	printf("Menu IMAGIMP\n");
	
}
