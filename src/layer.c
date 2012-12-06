#include "layer.h"
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
	
	l->lutList = NULL;
	
	return l;
}

Layer* createEmptyLayer(int w, int h){
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = createEmptyImg(w,h);
	l->opacity = 0.0;
	l->operation = MULTIPLICATION;
	
	l->lutList = NULL;
	
	return l;
}

//Surcouche de free au cas où l'implémentation de Layer changerait
void freeLayer(Layer* l){
	free(l);
}

void dumpLayer(Layer* l){
	if(l == NULL) return;
	
	printf("\n-----------Affichage Layer ID n° %d-----------\n\n", l->id);
	printf("Address : %p\n", l);
	printf("Image source : %s\nOpacity : %f\n", l->imgSource->name, l->opacity);
	printf("Opération : ");
	switch(l->operation){
		case SUM : printf("SUM\n");
		break;
		case MULTIPLICATION : printf("MULTIPLICATION\n");
		break;
	}
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

void setOpacity(Layer* lay, double newOpa){
	if(lay == NULL) return;
	lay->opacity = newOpa;
}

void setLayerOperation(Layer* lay, LAYER_OP newOp){
	if(lay == NULL) return;
	lay->operation = newOp;
}

Image* generateFinalImage(Layer* lay){
	if(lay == NULL) return NULL;
	
	//TO DO

	while(nextCell(lay->lutList) != NULL){
		
	}
	return NULL;
}








