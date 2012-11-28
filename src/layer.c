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
	}
}
