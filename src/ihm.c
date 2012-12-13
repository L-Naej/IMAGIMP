#include "ihm.h"
#include <ctype.h>
#include <stdlib.h>
#include <interface.h>
#include <string.h>
#include "layer.h"
#include "layersManager.h"

/**
 * Fonction interne utilisée pour trouvé le prochain LUT
 * demandé en ligne de commande pour le premier Layer.
 * Voir sujet du projet section 2.3.4 pour plus d'infos.
 */
int findNextLut(int argc, char** argv, int index, Lut** lt);

//Pour pouvoir libérer la mémoire on doit conserver le 
//pointeur entre deux appels de cette fonction
Image* finalImage = NULL; 

void initIHM(){
	fixeFonctionClavier(keyboardListener);	
	fixeFonctionClavierSpecial(keyboardSpecialListener);
	
	//Fonction appelée lors de la fermeture du programme
	atexit(exitProgramClean);
}


Layer* parseCmdLine(int argc, char** argv){
	if(argv == NULL) return false;
	
	if(argc < 2){
		fprintf(stderr, "Utilisation du programme : imagimp monimage.ppm [<code_lut>[_<param1>]*]*\n");
		return false;
	}
	
	Image* img = loadImage(argv[1]);
	int index = 0;
	Lut* currentLut = NULL;
	
	if(img == NULL){
		return NULL;
	}
	
	Layer* firstLay = createLayer(img, 1.0, SUM);//2 derniers params sans importance
	
	if(firstLay == NULL){
		freeImage(img);
		return NULL;
	}
	
	//Si pas de code LUT
	if(argc < 3) return firstLay;
	
	index = 2;
	
	index = findNextLut(argc, argv, index, &currentLut);
	do{
		if(currentLut)
			addLut(firstLay, currentLut);
	}while(index < argc);
	
	return firstLay;
}

int findNextLut(int argc, char** argv, int index, Lut** currentLut){
	LUT_FUNCTION ltFunc = NEUTRAL;
	int lutFuncValue = 0;
	
	//Vaudra true si la ligne de commande est mal formatée
	bool parseError = false;
	
	if(strcmp("ADDLUM", argv[index]) == 0){
		ltFunc = ADDLUM;
		//S'il existe encore un argument et que c'est un nombre
		//il s'agit de la valeur du lut
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (ADDLUM a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("DIMLUM", argv[index]) == 0) {
		ltFunc = DIMLUM;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (DIMLUM a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("ADDCON", argv[index]) == 0){
		ltFunc = ADDCON;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (ADDCON a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("DIMCON", argv[index]) == 0){
		ltFunc = DIMCON;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (DIMCON a besoin d'une valeur)
		else parseError = true;
	}
	else if(strcmp("INVERT", argv[index]) == 0){
		ltFunc = INVERT;
	}
	else if(strcmp("SEPIA", argv[index]) == 0){
		ltFunc = SEPIA;
		if(index < argc && isdigit(argv[++index][0]) ){
			lutFuncValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (SEPIA a besoin d'une valeur)
		else parseError = true;
	}
	else{ 
		fprintf(stderr,"Code lUT inconnu : %s.\n", argv[index]);
		parseError = true;
	}
	
	
	
	if(!parseError){
		*currentLut = createLut(ltFunc, lutFuncValue);
	}
	else{
		*currentLut = NULL;
		printf("Erreur dans la ligne de commande autour du mot n° %d\n", index);
	}
	
	index++;
	return index;
}

void displayCurrentLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return;
	displayImage(((Layer*)currentData(layerList))->imgSource);	
}

void displayImage(const Image* img){
	if(img == NULL || img->arrayRGB == NULL) return;
	
	actualiseImage(img->arrayRGB);
	
}

//TO DO
void printState(){
	extern List* layerList;
	Layer* lay = currentLayer(layerList);
	//Si on était en tête de liste
	if(lay == NULL){
		lay = nextLayer(layerList);
		//On ne veut pas bouger le curseur
		previousLayer(layerList);
	} 
	
	printf("\n----Position actuelle----\n");
	printf("Calque n°%d\n", layerList->position);
	printf("Image source : ");
	if(lay->imgSource->name != NULL)
		printf("%s\n", lay->imgSource->name);
	else
		printf(" Calque Vierge\n");
	printf("Opacité : %lf\n", lay->opacity);
	printf("Opération : ");
	if(lay->operation == SUM) printf("Somme\n");
	else printf("Multiplication\n");
	
	printf("Afficher les LUT quand ce sera implémenté.\n");
}

void userAddEmptyLayer(List* layerList){
	Layer* newLay = NULL;
	//Création du layer et définition de ses champs
	int width = currentLayer(layerList)->imgSource->width;
	int height = currentLayer(layerList)->imgSource->height;
	
	newLay = createEmptyLayer(width, height);
	if(newLay == NULL){
		fprintf(stderr,"Erreur d'allocation mémoire. Création du calque annulée.\n");
		return;
	}

	setLayerOpacity(newLay, 0.0);
	setLayerOperation(newLay, MULTIPLICATION);
	
	//Ajout du layer à la liste des layers
	//AddLayer fait pointer la liste sur le dernier calque,
	//et détecte si la taille de l'image est valide.
	//On n'a donc pas à s'en soucier
	if(addLayer(layerList, newLay)){
		printf("L'ajout du calque a réussi.\n");
		displayCurrentLayer(layerList);
	}
	else{
		printf("L'ajout du calque a échoué.\n");
		free(newLay->imgSource);
		free(newLay);
	}
}

void userAddLayer(List* layerList){

	int maxNameLenght;
	char* fileChosen = NULL;
	Image* img = NULL;
	Layer* newLay;
	double opacity;
	LAYER_OP operation; 
	int strLenght = 0;
	
	
	//Choix de l'image
	printf("Entrez le nom de l'image source voulue parmis celles qui vous sont proposées :\n");
	maxNameLenght = printDirectory("./images");
	if(maxNameLenght == 0){
		fprintf(stderr,"Erreur lors de l'affichage du dossier images (ou dossier vide). Abandon de l'ajout de calque.\n");
		return;
	}
	strLenght = strlen("./images/") + maxNameLenght + 1;
	
	fileChosen = (char*) calloc(strLenght,sizeof(char));
	
	if(fileChosen == NULL){
		fprintf(stderr, "Erreur allocation mémoire.\n");
		return;
	}
	
	printf("=> ");
	strcpy(fileChosen, "./images/");
	clearStdin();
	//+1 car sinon fgets s'arrête à maxNameLenght-1
	fgets(fileChosen + strlen("./images/"), maxNameLenght+1, stdin);
	//fgets copie le retour à la ligne si le nom du fichier est plus
	//court que maxNameLenght, il faut le supprimer sinon
	//le nom du fichier est invalide
	char* brLine = NULL;
	brLine = strchr(fileChosen,'\n');
	if(brLine != NULL)
		*brLine = '\0';
	
	//Choix opacité
	opacity = userSetOpacity();
	
	//Choix opération
	operation = userSetLayerOp();
	
	//Chargement de l'image
	img = loadImage(fileChosen);
	
	//Ne sert plus
	free(fileChosen);
	if(img == NULL){
		fprintf(stderr, "Erreur lors du chargement de l'image.");
		fprintf(stderr, " Création du calque annulée.\n");
		return;
	} 
	
	//Création du layer et définition de ses champs
	newLay = createLayer(img, opacity, operation);
	if(newLay == NULL){
		fprintf(stderr,"Erreur d'allocation mémoire. Création du calque annulée.\n");
		free(img);
		return;
	}
	
	newLay->imgSource = img;
	setLayerOpacity(newLay, opacity);
	setLayerOperation(newLay, operation);
	
	//Ajout du layer à la liste des layers
	//AddLayer fait pointer la liste sur le dernier calque,
	//l'on n'a donc pas à s'en soucier
	if(addLayer(layerList, newLay)){
		printf("L'ajout du calque a réussi.\n");
		displayCurrentLayer(layerList);
	}
	else{
		printf("L'ajout du calque a échoué.\n");
		free(img);
		free(newLay);
	}
	
}

double userSetOpacity(){
	double opacity;
	
	clearStdin();
	printf("\nDéfinissez l'opacité du calque (entre 0 et 1 compris) : ");
	clearStdin();
	scanf("%lf", &opacity);
	while(opacity > 1.0 || opacity < 0.0){
		fprintf(stderr, "Opacité rentrée non comprise entre 0 et 1");
		printf("\nDéfinissez l'opacité du calque (entre 0 et 1 compris) : ");
		clearStdin();
		scanf("%lf", &opacity);
	}
	
	
	return opacity;
}

LAYER_OP userSetLayerOp(){
	int opNum;
	LAYER_OP operation;
	
	printf("\nChoisissez l'opération de mélange du calque (entrez le nombre correspondant) : \n");
	printf("%d.Somme\n%d.Multiplication\n=>", SUM, MULTIPLICATION);
	clearStdin();
	scanf("%d", &opNum);
	while(opNum != SUM && opNum != MULTIPLICATION){
		printf("\nChoisissez l'opération de fusion du calque (entrez le nombre correspondant) : \n");
		printf("%d.Somme\n%d.Multiplication\n=>", SUM, MULTIPLICATION);
		clearStdin();
		scanf("%d", &opNum);
	}
	
	operation = opNum;
	return operation;
}

bool userDelCurrentLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return false;
	//Impossible de supprimer s'il n'y a qu'un seul calque
	if(layerList->size == 1) return false;
	
	Cell* c = currentCell(layerList);
	
	//Suppression du calque de la liste en vidant
	//complètement la mémoire
	freeCellInList(layerList, c);
	return true;
}

void userSaveFinalImage(List* layerList){
	char imgName[51] = "./images/";
	imgName[51] = '\0';
	printf("Sauvegarde de l'image finale.\n");
	printf("Entrez le nom de l'image que vous voulez sauvegarder (max 50 caractères) : \n");
	
	clearStdin();
	fgets(imgName + 9, 51, stdin);
	//fgets copie le retour à la ligne si le nom du fichier est plus
	//court que maxNameLenght, il faut le supprimer sinon
	//le nom du fichier est invalide
	char* brLine = NULL;
	brLine = strchr(imgName,'\n');
	if(brLine != NULL)
		*brLine = '\0';
		
	if(finalImage == NULL){
		generateFinalImage(layerList, &finalImage);
		imgAddName(finalImage, imgName);
	}
	if( ! saveImage(finalImage) ){
		fprintf(stderr, "Une erreur est survenue pendant la sauvegarde de l'image finale.\n");
	}
	else{
		printf("%s sauvegardée.\n", imgName);
	}
	
}

//TO DO
void keyboardListener(unsigned char c, int x, int y){
	//Définie dans imagimp.c
	extern List* layerList;
	
	//On ne pas apparemment pas déclarer une
	//variable dans un switch...
	
	double opacity;
	LAYER_OP operation;
	
	switch(c){
		//Touche échap
		case 27:
			exit(0);
		break;
		case 'a':
			userAddLayer(layerList);
			printState();
		break;
		case 'g':
			//Si elle existait déjà on la supprime (il y a peut-être
			//de nouveaux calques ou de nouveaux LUT)
			freeImage(finalImage);
			generateFinalImage(layerList, &finalImage);
			displayImage(finalImage);
			printf("Affichage du résultat final. ");
			printf("Appuyer sur \'c\' pour revenir au calque courant.\n");
		break;
		case 'c' :
			displayCurrentLayer(layerList);
			printState();
		break;
		case 'v' :
			userAddEmptyLayer(layerList);
			printState();
		break;
		case 'o' :
			opacity = userSetOpacity();
			setLayerOpacity(currentLayer(layerList), opacity);
			printf("Opacité du calque courant modifiée à %lf\n", opacity);
		break;
		case 'm' :
			operation = userSetLayerOp();
			setLayerOperation(currentLayer(layerList), operation);
			printf("Opération du calque courant modifiée.\n");
		break;
		case 'd': 
			//A faire tout de suite sinon opengl risque 
			//de tenter d'afficher une image d'un layer qui n'existe plus
			if(currentCell(layerList) != layerList->head){
				previousCell(layerList);
				displayCurrentLayer(layerList);
				nextCell(layerList);
			}
			if( userDelCurrentLayer(layerList) ){
				displayCurrentLayer(layerList);
				printf("Calque supprimé.\n");
				
			}
			else
				printf("Un seul calque est présent, vous ne pouvez pas le supprimer.\n");
			printState();
		break;
		case 's':
			userSaveFinalImage(layerList);
		break;
			
		default : printf("Touche non reconnue.\n");
		break;
	}
}

//A FINIR
void keyboardSpecialListener(int c, int x, int y){
	//Définie dans imagimp.c
	extern List* layerList;
	Layer* lay = NULL;
	
	switch(c){
		case GLUT_KEY_LEFT : 
			lay = previousLayer(layerList);
			//Si on était sur le premier layer, lay == NULL
			//On se repositionne sur le premier
			if(lay == NULL){
				nextLayer(layerList);
				return;
			}
			displayImage(lay->imgSource);
			printState();
		break;
		case GLUT_KEY_RIGHT :
			lay = nextLayer(layerList);
			//Si on était sur le dernier layer, lay == NULL
			if(lay == NULL) return;
			displayImage(lay->imgSource);
			printState();
		break;
		default : printf("Touche non reconnue.\n");
		break;	
	}
}

void exitProgramClean(){
	//Définie dans imagimp.c
	extern List* layerList;
	if(!isListEmpty(layerList))
		freeListComplete(layerList);
}
















