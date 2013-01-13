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
	
	printf("\n\n\tAppuyez sur ESPACE pour afficher la liste des commandes.\n");
	printState();
	
	Image* img = ((Layer*)currentData(layerList))->imgFinale;
	
	initGLIMAGIMP(img->width, img->height, img->arrayRGB);
	
	
	
	return 0;
}
