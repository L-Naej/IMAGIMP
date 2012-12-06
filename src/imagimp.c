#include "layer.h"
#include "ihm.h"

//A TERMINER, pour l'instant prototype de test
Layer* initLayer(argc,argv){
	if(argc < 2) {
		displayUsage();
		exit(-1);
	}	
	
	Layer* initialLay = NULL;
	Image* img = NULL;
	Lut* currentLut = NULL;
	unsigned char* lutInput = NULL;
	LUT_FUNCTION ltFunc;
	unsigned char lutFuncValue = 0;
	
	img = loadImage(argv[1]);
	
	if(img == NULL){
		fprintf(stderr, "Erreur lors du chargement de l'image, le programme va quitter.\n");
		exit(-1);
	}
	
	//Pour le premier layer les deux derniers arguments sont sans importance
	initialLay = createLayer(img, 1.0, SUM);
	
	//Début boucle
	//Analayse code LUT
	//Si code valide, analyse des paramètres éventuels
	//Fin boucle
	
	//Si aucun LUT n'est appliqué on retourne le layer tel quel
	if(argc < 3) return intialLay;
	
	switch(argv[2]){
		//Attend un paramètre
		case "ADDLUM" : 
			ltFunc = ADDLUM;
			lutInput = img->histoGrey;
		break;
		case "DIMLUM" :
			ltFunc = DIMLUM;
			lutInput = img->histoGrey;
		break;
		case "ADDCON" :
			ltFunc = ADDCON;
			lutInput = img->histoGrey;
		break;
		case "DIMCON" :
			ltFunc = DIMCON;
			lutInput = img->histoGrey;
		break;
		case "INVERT" :
			ltFunc = INVERT;
			lutInput = img->histoGrey;
		break;
		case "SEPIA" :
			ltFunc = SEPIA;
			lutInput = img->histoRGB;
		break;
		default : 
			fprintf(stderr,"Code lUT inconnu.\n");
			return initialLay;
		break;
	}
	if(argc == 4) lutFuncValue = argv[3];  
	currentLut = createLut(lutInput, img->maxValue, img->maxValue, ltFunc, atoi(argv[3]));
	addLut(initialLay, currentLut);
	
	return initialLay;
}



int main(int argc, char** argv){
	
	//Créé un layer à partir des arguments passés en ligne de commande
	Layer* fisrtLayer = initLayer(argc,argv);
	
	initGLIMAGIMP(test->width,test->height,test->arrayRVB);
	
	while(1){
		displayMenu();
		waitAction();
	}
	
}
