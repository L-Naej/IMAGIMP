#include "layersManager.h"
#include "ihm.h"
#include "image.h"
#include "review.h"
#include <math.h>

List* initLayersList(int argc, char** argv){
	Layer* firstLay = NULL;
	//Parse la ligne de commande pour initialiser le premier layer
	firstLay = parseCmdLine(argc, argv);
	
	List* layerList = NULL;
	
	if(firstLay != NULL){
		layerList = createList(LAYER, firstLay);
	}
	else{
		fprintf(stderr, "Impossible d'initialiser la liste des calques, le programme va quitter.\n");
		exit(-1);
	}
	
	return layerList;
}

bool addLayer(List* layerList, Layer* newLay){
	if(layerList == NULL || newLay == NULL || newLay->imgSource == NULL)
		return false;
		
	int width, height;
	Layer* firstLay = NULL;
	ListState* state;
	
	//Vérifie que newLay possède les bonnes dimensions
	//Si des calques existent déjà dans la liste
	if(!isListEmpty(layerList)){
		state = saveListState(layerList);
		goToHeadList(layerList);
		firstLay = (Layer*) nextData(layerList);
		width = firstLay->imgSource->width;
		height = firstLay->imgSource->height;
		
		if(newLay->imgSource->width != width 
		|| newLay->imgSource->height != height){
			fprintf(stderr, "Le calque que vous essayez d'ajouter ne possède pas les mêmes dimensions que le calque initial.\n");
			restoreListState(state);
			free(state);
			return false;		
		}
	}
	
	goToBottomCell(layerList);
	

	insertAfterCell(layerList, newLay);
	
	//La fonction place le curseur en fin de liste
	//Sur le dernier layer ajouté
	goToBottomCell(layerList);
	
	return true;
}

Layer* nextLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return NULL;
	return (Layer*) nextData(layerList);	
}

Layer* previousLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return NULL;
	return (Layer*) previousData(layerList);	
}

Layer* currentLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return NULL;
	return (Layer*) currentData(layerList);	
}

Layer* delCurrentLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return false;
	//Impossible de supprimer s'il n'y a qu'un seul calque
	if(layerList->size == 1) return false;
	
	Cell* c = currentCell(layerList);
	Layer* curLay = NULL;
	
	//Suppression du calque de la liste gardant
	//le calque en mémoire pour historique
	curLay = delCellInList(layerList, c);
	return curLay;
}

Layer* delLayer(List* layerList, Layer* lay){
	if(layerList == NULL || lay == NULL || layerList->type != LAYER)
		return false;
	
	//On va au layer s'il existe et on le supprime
	if(goToData(layerList, (void*) lay) != -1 ){
		return delCellInList(layerList, currentCell(layerList));
		
	}
	else return NULL;
}

//Cette fonction suppose que toutes les images ont les mêmes
//dimensions et peut donc se baser sur le calque courant
//pour les récupérer
bool generateFinalImage(List* layerList, Image** finalImage){
	if(layerList == NULL || layerList->type != LAYER || finalImage == NULL)
		return false;
	
	//Alloue l'espace pour stocker l'image finale
	int width, height, maxValue;
	width = currentLayer(layerList)->imgFinale->width;
	height = currentLayer(layerList)->imgFinale->height;
	maxValue = currentLayer(layerList)->imgFinale->maxValue;
	*finalImage = createEmptyImg(width, height, maxValue);
	
	//On sauvegarde l'emplacement actuel
	//du programme dans la liste des calques
	ListState* state = saveListState(layerList);
	
	goToBottomCell(layerList);
	
	//RECURSIVITE
	genFinalImageRecur(layerList, currentData(layerList), *finalImage);
	
	//On remet le curseur à son ancien emplacement
	restoreListState(state);
	free(state);
	
	return true;
}

/**
 * Fonctionnement récursif, voir formule de math dans sujet du projet.
 * Algo : 
 * final = image finale
 * arrayRGB = image du layer courant
 * alpha = opacité du layer courant
 * 
 * if previousLayer existe
 * 		if addition:
			final = recur(i-1) + alpha * arrayRGB
		else if multiplication:
			final = (1 - alpha)recur(i-1) + alpha * arrayRGB
 * else
	final = arrayRGB;
 * return final (implicite);
 */
void genFinalImageRecur(List* list, Layer* lay, Image* finalImage){
	Layer* pre = NULL;
	long int i = 0;
	long int nPix = finalImage->width*finalImage->height;
	short tmp;
	
	if ( (pre = previousData(list)) != NULL ){
		genFinalImageRecur(list, pre, finalImage);
		
		if(lay->operation == SUM){
			for(i = 0; i < nPix*NB_COL_COMP; ++i){
				tmp = finalImage->arrayRGB[i] + (short)(ceil(lay->opacity * lay->imgFinale->arrayRGB[i]));
				if(tmp > finalImage->maxValue) tmp = finalImage->maxValue;
				else if(tmp < 0) tmp = 0;
				finalImage->arrayRGB[i] = tmp;
			}
		}
		else if(lay->operation == MULTIPLICATION){
			for(i = 0; i < nPix*NB_COL_COMP; ++i){
				tmp = (short)(ceil((1-lay->opacity)*finalImage->arrayRGB[i])) + (short)(ceil(lay->opacity * lay->imgFinale->arrayRGB[i]));
				if(tmp > finalImage->maxValue) tmp = finalImage->maxValue;
				else if(tmp < 0) tmp = 0;
				finalImage->arrayRGB[i] = tmp;
			}
		}
	}
	//Condition de fin de la récursivité : on est sur le premier layer
	else{
		for(i = 0; i < nPix*NB_COL_COMP; ++i){
			finalImage->arrayRGB[i] = lay->imgFinale->arrayRGB[i];
		}
	}
}
