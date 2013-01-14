#include "ihm.h"
#include "layersManager.h"
#include "list.h"
#include "utils.h"
#include <interface.h>
#include <outils.h>

//Variables globales

/**
 * Représente la liste de calques utilisée par le programme.
 */
List* layerList;

void mondessin() {
	/* Boutons principaux */
	fixeCouleur(0.2,0.2,0.2);
	drawCarre(0.73,0.86,0.81,0.96);
	drawCarre(0.82,0.86,0.90,0.96);
	drawCarre(0.91,0.86,0.99,0.96);
	
	drawCarre(0.73,0.76,0.86,0.85);
	drawCarre(0.87,0.76,0.99,0.85);
	
	
	drawCarre(0.73,0.66,0.99,0.71);
	drawCarre(0.73,0.60,0.99,0.65);
	
	drawCarre(0.73,0.50,0.86,0.59);
	drawCarre(0.87,0.50,0.99,0.59);
	
	drawCarre(0.73,0.44,0.99,0.49);
	drawCarre(0.73,0.38,0.99,0.43);
	
	
	drawCarre(0.73,0.28,0.86,0.33);
	drawCarre(0.87,0.28,0.991,0.33);
	
	drawCarre(0.73,0.22,0.99,0.27);
	drawCarre(0.73,0.16,0.99,0.21);
	
	drawCarre(0.73,0.03,0.86,0.13);
	drawCarre(0.87,0.03,0.99,0.13);
	
	/* Petits Boutons */
	fixeCouleur(0.5,0.5,0.5);
	drawCarre(0.735,0.765,0.765,0.805);
	drawCarre(0.825,0.765,0.855,0.805);
	drawCarre(0.875,0.765,0.905,0.805);
	drawCarre(0.955,0.765,0.985,0.805);
	
	drawCarre(0.735,0.505,0.765,0.545);
	drawCarre(0.825,0.505,0.855,0.545);
	drawCarre(0.875,0.505,0.905,0.545);
	drawCarre(0.955,0.505,0.985,0.545);
	
	/* Intitulés des Boutons */
	fixeCouleur(1.0,1.0,1.0);
	writeString(0.823,0.975,"EFFECTS");
	writeString(0.735,0.91,"INVERT");
	writeString(0.835,0.91,"SEPIA");
	writeString(0.925,0.91,"B & W");
	writeString(0.74,0.825,"LUMINOSITY");
	writeString(0.744,0.78,"-");
	writeString(0.836,0.78,"+");
	writeString(0.885,0.825,"CONTRAST");
	writeString(0.884,0.78,"-");
	writeString(0.965,0.78,"+");
	writeString(0.825,0.725,"LAYERS");
	writeString(0.755,0.68,"ADD NEW EMPTY LAYER");
	writeString(0.79,0.62,"ADD NEW LAYER");
	writeString(0.755,0.56,"OPACITY");
	writeString(0.744,0.52,"-");
	writeString(0.836,0.52,"+");
	writeString(0.915,0.56,"MIX");
	writeString(0.884,0.52,"+");
	writeString(0.965,0.52,"x");
	writeString(0.76,0.46,"SHOW CURRENT LAYER");
	writeString(0.75,0.40,"DELETE CURRENT LAYER");
	writeString(0.84,0.35,"MORE");
	writeString(0.77,0.30,"UNDO");
	writeString(0.87,0.30,"UNDO EFFECT");
	writeString(0.795,0.24,"SHOW HISTORY");
	writeString(0.78,0.18,"SHOW HISTOGRAM");
	writeString(0.75,0.09,"GENERATE");
	writeString(0.735,0.065,"FINAL IMAGE");
	writeString(0.91,0.07,"SAVE");
}

int main(int argc, char** argv){
	
	//Définition des fonctions callback de l'IHM
	initIHM();
	
	//Initialisation de la liste des calques
	layerList = initLayersList(argc, argv);
	
	printf("\n\n\tAppuyez sur ESPACE pour afficher la liste des commandes.\n");
	printState();
	
	Image* img = ((Layer*)currentData(layerList))->imgFinale;
	
	if (img->width<=512 && img->height<=512){
		fixeFonctionClicSouris(clickMouse);
		fixeFonctionDessin(mondessin);
		initGLIMAGIMP_IHM(img->width,img->height,img->arrayRGB,712,512);
	} 
	else initGLIMAGIMP(img->width, img->height, img->arrayRGB);
	
	return 0;
}
