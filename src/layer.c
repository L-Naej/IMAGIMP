#include "layer.h"
#include "lut.h"
#include "lutfunction.h"//Pour imgToGray
#include "ihm.h" //Pour displayImage
#include <stdlib.h>

/**
 * Variable globale pour générer l'id des layers.
 */
int cntLayerId = 0;

//Fonction privée d'ajout du Lut neutre en début de liste
bool addNeutralLut(Layer* lay, int maxValue){
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
	
	lay->lutList = createList(LUT, neutralLut);
	 if(lay->lutList == NULL){
	 	fprintf(stderr, "Impossible d'initialiser la liste des lut.\n");
	 	return false;
	 }

	return true;
}

Layer* createLayer(Image* source, double opa, LAYER_OP operation){
	if(source == NULL) return NULL;
	
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = source;
	l->opacity = opa;
	l->operation = operation;
	l->lutList = NULL;
	
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
	
	//Génération des histogrammes (au début histo final = histo source)
	l->histoSource = NULL;
	l->histoFinal = NULL;
	l->histoSource = createHistogram(l->imgSource);
	
	if(l->histoSource == NULL){
		fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour l'histogramme de l'image du calque d'id %d.\n", l->id);
	}
	
	//Au départ aucun effet appliqué donc l'histogramme de départ
	//et le même que celui d'arrivée.
	l->histoFinal = copyImage(l->histoSource);
	if(l->histoFinal == NULL){
		fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour l'histogramme de l'image du calque d'id %d.\n", l->id);
	}
	
	return l;
}

Layer* createEmptyLayer(int w, int h){
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->lutList = NULL;
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
	
	//Génération des histogrammes (au début histo final = histo source)
	l->histoSource = NULL;
	l->histoFinal = NULL;
	l->histoSource = createHistogram(l->imgSource);
	
	if(l->histoSource == NULL){
		fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour l'histogramme de l'image du calque d'id %d.\n", l->id);
	}
	
	//Au départ aucun effet appliqué donc l'histogramme de départ
	//et le même que celui d'arrivée.
	l->histoFinal = copyImage(l->histoSource);
	if(l->histoFinal == NULL){
		fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour l'histogramme de l'image du calque d'id %d.\n", l->id);
	}
	
	return l;
}

//Libère la mémoire prise par un layer
void freeLayer(Layer* l){
	if(l== NULL) return;
	freeImage(l->imgSource);
	freeImage(l->imgFinale);
	freeImage(l->histoSource);
	freeImage(l->histoFinal);
	
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


bool addLut(Layer* lay, LUT_FUNCTION function, int functionValue){
	Lut* lt = NULL;
	Channels* input = NULL;
	if(lay == NULL || lay->lutList == NULL)
		return false;
	
	goToBottomCell(lay->lutList);
	input = ((Lut*)currentData(lay->lutList))->channels;
	

	lt = createLut(input, function, functionValue, lay->imgFinale->maxValue);
	if(lt == NULL) return false;
	
	insertBottomCell(lay->lutList, lt);
	goToBottomCell(lay->lutList);
	
	//On appelle la fonction qui teste si sépia est présent
	//car cela implique des traitements spéciaux
	applyLuts(lay);

	return true;
}

Lut* delLastLut(Layer* lay){
	if(lay == NULL) return NULL;
	Lut* lutToDelete = NULL;
	
	Cell* last = goToBottomCell(lay->lutList);
	lutToDelete = (Lut*)last->userData;
	
	//S'il n'y a plus de lut à supprimer (le neutral doit rester)
	if( lutToDelete->function == NEUTRAL ) return NULL;
	
	delBottomCell(lay->lutList);
	
	applyLuts(lay);
	
	//On retourne le lut pour pouvoir le stocker dans l'historique
	return lutToDelete;
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


void applyLuts(Layer* lay){
	if(lay == NULL) return;
	
	//Indique si l'effet sépia est aparu dans la chaîne
	bool sepia = false;
	
	//S'il n'y a que le Lut neutre, c'est qu'on doit afficher l'image de base
	//Si cette fonction est appelée suite à une suppression, 
	//il faut tout de même restaurer l'image de base
	if(lay->lutList->size  == 1){
		displayImage(lay->imgSource);//Evite segfault OpenGL
		free(lay->imgFinale);
		lay->imgFinale = copyImage(lay->imgSource);
		return;	
	} 

	Image* tmp = copyImage(lay->imgSource);
	
	if(tmp == NULL){
		fprintf(stderr, "\nImpossible d'allouer la mémoire nécessaire pour créer l'image temporaire. L'application des effets a échoué.\n");
		return;
	} 
	

	/* Le principe est le suivant :
	   s'il y a l'effet sépia, on recalcule le
	   pipeline des LUT APRES le lut effet sépia
	   en ayant converti l'image en niveaux de gris.
	*/
	Lut* currentLut = NULL;
	Lut* previousLut = NULL; 
	
	goToHeadList(lay->lutList);
	previousLut = (Lut*) nextData(lay->lutList);
	while( ( currentLut = (Lut*) nextData(lay->lutList) ) != NULL){
		
		if(currentLut->function == SEPIA || currentLut->function == GRAY){
			applyLutToImg(tmp, tmp, previousLut);
			imgToGray(tmp,tmp);
			sepia = true;
		}
		
		else if(sepia){
			regenerateLut(&currentLut, previousLut->channels, tmp);
		}
		
		previousLut = currentLut;
		
	}
	currentLut = (Lut*) currentData(lay->lutList);
	
	displayImage(lay->imgSource);//Evite segfault OpenGL
	applyLutToImg(tmp, lay->imgFinale, currentLut);
	
	freeImage(tmp);
	
	//On génère l'histogramme final
	if( ! generateFinalHistogram(lay) ){
		fprintf(stderr, "\nErreur : impossible de calculer l'histogramme final...\n");
	}
}

bool generateFinalHistogram(Layer* lay){
	if(lay == NULL) return false;
	
	//Evite segFault au cas où l'histo est déjà affiché
	displayImage(lay->imgFinale);
	freeImage(lay->histoFinal);
	lay->histoFinal = createHistogram(lay->imgFinale);
	if(lay->histoFinal == NULL) return false;
	
	return true;
}
