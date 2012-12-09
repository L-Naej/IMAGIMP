#include "layersManager.h"
#include "ihm.h"
#include "image.h"
#include <interface.h> //Pour debug
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
	if(layerList == NULL || newLay == NULL) return false;
	
	goToBottomCell(layerList);
	insertAfterCell(layerList, newLay);
	
	//La fonction place le curseur en fin de liste
	//Sur le dernier layer ajouté
	goToBottomCell(layerList);
	
	return true;
}

bool generateFinalImage(List* layerList, Image* finalImage){
	if(layerList == NULL || finalImage == NULL) return false;
	
	//On sauvegarde l'emplacement actuel
	//du programme dans la liste des calques
	Cell* savedCursor = currentCell(layerList);
	
	goToBottomCell(layerList);
	
	//RECURSIVITE
	genFinalImageRecur(layerList, currentData(layerList), finalImage);
	
	//On remet le curseur à son ancien emplacement
	layerList->cursor = savedCursor;
	
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
				tmp = finalImage->arrayRGB[i] + (short)(ceil(lay->opacity * lay->imgSource->arrayRGB[i]));
				if(tmp > finalImage->maxValue) tmp = finalImage->maxValue;
				else if(tmp < 0) tmp = 0;
				finalImage->arrayRGB[i] = tmp;
			}
		}
		else if(lay->operation == MULTIPLICATION){
			for(i = 0; i < nPix*NB_COL_COMP; ++i){
				tmp = (short)(ceil((1-lay->opacity)*finalImage->arrayRGB[i])) + (short)(ceil(lay->opacity * lay->imgSource->arrayRGB[i]));
				if(tmp > finalImage->maxValue) tmp = finalImage->maxValue;
				else if(tmp < 0) tmp = 0;
				finalImage->arrayRGB[i] = tmp;
			}
		}
	}
	//Condition de fin de la récursivité : on est sur le premier layer
	else{
		for(i = 0; i < nPix*NB_COL_COMP; ++i){
			finalImage->arrayRGB[i] = lay->imgSource->arrayRGB[i];
		}
	}
}
