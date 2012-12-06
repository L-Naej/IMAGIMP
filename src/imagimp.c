#include "layer.h"
#include "ihm.h"

//A TERMINER
Layer* initLayer(argc,argv){
	if(argc < 2) {
		displayUsage();
		exit(-1);
	}	
	
	Layer* initialLay = NULL;
	Image* img = NULL;
	
	img = loadImage(argv[1]);
	
	if(img == NULL){
		fprintf(stderr, "Erreur lors du chargement de l'image, le programme va quitter.\n");
		exit(-1);
	}
	
	//Pour le premier layer les deux derniers arguments sont sans importance
	initialLay = createLayer(img, 1.0, SUM);
}



int main(int argc, char** argv){
	
	//Créé un layer à partir des arguments passés en ligne de commande
	Layer* fisrtLayer = initLayer(argc,argv);
	
	while(1){
		displayMenu();
		waitAction();
	}
	
}
