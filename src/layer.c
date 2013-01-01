#include "layer.h"
#include "lut.h"
#include <stdlib.h>

/**
 * Variable globale pour générer l'id des layers.
 */
int cntLayerId = 0;

Layer* createLayer(Image* source, double opa, LAYER_OP operation){
	if(source == NULL) return NULL;
	
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = source;
	l->opacity = opa;
	l->operation = operation;
	
	Lut* neutralLut = createLut(NEUTRAL,0);
	
	if(neutralLut == NULL) return NULL;
	
	l->lutList = NULL;
	addLut(l,neutralLut);
	
	return l;
}

Layer* createEmptyLayer(int w, int h){
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = createEmptyImg(w,h, DEFAULT_MAX_VAL);
	if(l->imgSource == NULL){
		free(l);
		return NULL;
	}
	
	l->opacity = 0.0;
	l->operation = MULTIPLICATION;
	
	l->lutList = NULL;
	
	return l;
}

//Libère la mémoire prise par un layer
void freeLayer(Layer* l){
	if(l== NULL) return;
	freeImage(l->imgSource);
	free(l);
}

void dumpLayer(Layer* l){
	if(l == NULL) return;
	
	printf("\n-----------Affichage Layer ID n° %d-----------\n\n", l->id);
	printf("Address : %p\n", l);
	printf("Opération : ");
	switch(l->operation){
		case SUM : printf("SUM\n");
		break;
		case MULTIPLICATION : printf("MULTIPLICATION\n");
		break;
	}
	printf("Opacity : %f\n", l->opacity);
	printf("Image source : %s\n", l->imgSource->name);
	dumpImage(l->imgSource);
}


bool addLut(Layer* lay, Lut* lt){
	if(lay == NULL || lt == NULL)
		return false;
		
	if(lay->lutList == NULL){
		lay->lutList = createList(LUT, lt);
	 	if(lay->lutList == NULL)
	 		return false;
	 	return true;
	}
	
	insertBottomCell(lay->lutList, lt);
	return true;
}

void setLayerOpacity(Layer* lay, double newOpa){
	if(lay == NULL) return;
	if(newOpa > 1.0) newOpa = 1.0;
	else if(newOpa < 0.0) newOpa = 0.0;
	
	lay->opacity = newOpa;
}

void setLayerOperation(Layer* lay, LAYER_OP newOp){
	if(lay == NULL) return;
	lay->operation = newOp;
}

Image* applyLuts(Layer* lay){
	if(lay == NULL) return NULL;
	if(isListEmpty(lay->lutList)){
	return lay;
	}
	
	Lut* currentLut = NULL;
	Lut* previousLut = NULL; 
	
	while(nextCell(lay->lutList) != NULL){
		if ((lay->lutList)->position == 0){
			goToPosition(lay->lutList,1);
		}
		else{
		previousLut =(Lut*) previousData(lay->lutList);
		currentLut = (Lut*) currentData(lay->lutList);
		applyLut(currentLut->inputArrayRGB, previousLut->function, previousLut->valueEffect);
		}
	}
	return NULL;
}

