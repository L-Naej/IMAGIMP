#include "layer.h"
#include "lut.h"
#include <stdlib.h>

/**
 * Variable globale pour générer l'id des layers.
 */
int cntLayerId = 0;

//Fonction privée d'ajout du Lut neutre en début de liste
bool addNeutralLut(Layer* lay, int maxValue){
	int i = 0;
	Channels* ntLutInput = NULL;
	

	
	//Création de l'input pour le LUT Neutre	
	ntLutInput = allocChannels(maxValue+1);
	if(ntLutInput == NULL){
		return false;
	} 
	
	Lut* neutralLut = createLut(ntLutInput, NEUTRAL,0, maxValue);
	
	if(neutralLut == NULL){
		freeLayer(lay);
		return false;
	} 
	
	bool success = addLut(lay,neutralLut);
	return success;
}

Layer* createLayer(Image* source, double opa, LAYER_OP operation){
	if(source == NULL) return NULL;
	
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = source;
	l->opacity = opa;
	l->operation = operation;
	
	//Allocation de la mémoire pour l'image finale (effets appliqués)
	l->imgFinale = NULL;
	l->imgFinale = copyImage(l->imgSource);
	if(l->imgFinale == NULL){
		fprintf(stderr, "Une erreur est survenue lors de l'allocation mémoire de l'image finale du layer %p.\n", l);
		freeLayer(l);
	}
	
	if(! addNeutralLut(l, source->maxValue) ){
		fprintf(stderr, "Une erreur est survenue lors de l'ajout du LUT neutre du layer %p.\n", l);
		freeLayer(l);
		return NULL;
	}
	
	return l;
}

Layer* createEmptyLayer(int w, int h){
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = NULL;
	l->imgFinale = NULL;
	l->imgSource = createEmptyImg(w,h, DEFAULT_MAX_VAL);
	l->imgFinale = createEmptyImg(w,h, DEFAULT_MAX_VAL);
	if(l->imgSource == NULL || l->imgFinale == NULL){
		freeLayer(l);
		return NULL;
	}
	
	l->opacity = 0.0;
	l->operation = MULTIPLICATION;
	
	if(! addNeutralLut(l, l->imgSource->maxValue) ){
		fprintf(stderr, "Une erreur est survenue lors de l'ajout du LUT neutre.\n");
		freeLayer(l);
		return NULL;
	}
	
	return l;
}

//Libère la mémoire prise par un layer
void freeLayer(Layer* l){
	if(l== NULL) return;
	freeImage(l->imgSource);
	freeImage(l->imgFinale);
	
	//Suppression des LUT
	freeListComplete(l->lutList);
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
	goToBottomCell(lay->lutList);
	//On applique le LUT au calque
	applyLutToImg(lay->imgFinale, (Lut*)currentData(lay->lutList));
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

/*
Image* applyLuts(Layer* lay){
	if(lay == NULL) return NULL;
	if(isListEmpty(lay->lutList)){
	return NULL;
	}
	
	Lut* currentLut = NULL;
	Lut* previousLut = NULL; 
	
	goToHeadList(lay->lutList);
	
	while(nextCell(lay->lutList) != NULL){
		previousLut =(Lut*) previousData(lay->lutList);
		currentLut = (Lut*) nextData(lay->lutList);
		applyLut(currentLut->inputArrayRGB, previousLut->function, previousLut->valueEffect);
	}
	return NULL;
}*/

