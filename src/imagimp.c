#include "ihm.h"
#include "layersManager.h"
#include "list.h"
#include "utils.h"
#include <interface.h>

//Variables globales

/**
 * Représente la liste de calques utilisée par le programme.
 */
List* layerList;

int main(int argc, char** argv){
	
	//Définition des fonctions callback de l'IHM
	initIHM();
	
	//Initialisation de la liste des calques
	layerList = initLayersList(argc, argv);
	
	
	
	//TEST
	/*
	Image* secImage = loadImage("images/Clown.256.ppm");
	invertPPMArray(secImage->arrayRGB, secImage->width, secImage->height);
	Layer* testLayer = createLayer(secImage, 1.0, SUM);
	
	addLayer(layerList, testLayer);

	Image* finalImage = createEmptyImg(width, height);
	finalImage->maxValue = imgSource->maxValue;
	
	if(generateFinalImage(layerList, finalImage)){
		initGLIMAGIMP(finalImage->width, finalImage->height, finalImage->arrayRGB);
	}
	*/
	//FIN TEST
	
	printf("\n\n\tAppuyez sur ESPACE pour afficher la liste des commandes.\n");
	printState();
	
	Image* img = ((Layer*)currentData(layerList))->imgFinale;
	initGLIMAGIMP(img->width, img->height, img->arrayRGB);
	
	
	return 0;
}
