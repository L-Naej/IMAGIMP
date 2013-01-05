#include "ihm.h"
#include <ctype.h>
#include <stdlib.h>
#include <interface.h>
#include <string.h>
#include "layer.h"
#include "layersManager.h"
#include "review.h"

/**
 * Fonction interne utilisée pour trouvé le prochain LUT
 * demandé en ligne de commande pour le premier Layer.
 * Voir sujet du projet section 2.3.4 pour plus d'infos.
 */
int findNextLut(int argc, char** argv, int index, LUT_FUNCTION* function, int* functionValue, Channels* input,  int maxValue, bool* parseError);

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
	int index = 0, functionValue = 0;
	Lut* currentLut = NULL;
	LUT_FUNCTION function = NEUTRAL;
	bool parseError = false;
	
	if(img == NULL){
		return NULL;
	}
	
	Layer* firstLay = createLayer(img, 1.0, SUM);//2 derniers params sans importance

	
	if(firstLay == NULL){
		freeImage(img);
		return NULL;
	}
	
	//Enregistrement du chargement de l'image dans l'historique
	if( !recordImgOperation(firstLay->imgSource, IM1)){
				fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");	
	}
	
	//Si pas de code LUT
	if(argc < 3) return firstLay;
	
	index = 2;
	
	//On se positionne sur le lut neutre
	currentLut = (Lut*) goToBottomCell(firstLay->lutList)->userData;
	//On parcourt les arguments et on ajoute les lut correspondants
	while(index < argc){
		index = findNextLut(argc, argv, index, &function, &functionValue, currentLut->channels, img->maxValue, &parseError);
		
		//Si ligne mal formatée on quitte avec message d'erreur
		if(parseError){ 
			fprintf(stderr, "Utilisation du programme : imagimp monimage.ppm [<code_lut>[_<param1>]*]*\n");
			exit(-1);
		}
		
		//Ajout du lut à la liste des lut du premier calque
		if(! addLut(firstLay, function, functionValue) ) {
			fprintf(stderr, "Impossible de charger le LUT demandé.\n");
		}
		//Enregistrement de l'opération dans l'historique
		else {
			goToBottomCell(firstLay->lutList);//Sécurité
			currentLut = (Lut*) currentData(firstLay->lutList);
			if ( !recordLutOperation(firstLay->lutList, firstLay, currentLut, LUT1) )
				fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");
		}
		
	}
	
	return firstLay;
}

int findNextLut(int argc, char** argv, int index, LUT_FUNCTION* function, int* functionValue, Channels* input, int maxValue, bool* parseError){
	
	if(strcmp("ADDLUM", argv[index]) == 0){
		*function = ADDLUM;
		//S'il existe encore un argument et que c'est un nombre
		//il s'agit de la valeur du lut
		if(index < argc-1 && isdigit(argv[++index][0]) ){
			*functionValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (ADDLUM a besoin d'une valeur)
		else{
			*parseError = true;
			fprintf(stderr, "ADDLUM nécessite un paramètre (nombre entier).\n");
		}
		
	}
	else if(strcmp("DIMLUM", argv[index]) == 0) {
		*function = DIMLUM;
		if(index < argc-1 && isdigit(argv[++index][0]) ){
			*functionValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (DIMLUM a besoin d'une valeur)
		else{
			*parseError = true;
			fprintf(stderr, "DIMLUM nécessite un paramètre (nombre entier).\n");
		}
	}
	else if(strcmp("ADDCON", argv[index]) == 0){
		*function = ADDCON;
		if(index < argc-1 && isdigit(argv[++index][0]) ){
			*functionValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (ADDCON a besoin d'une valeur)
		else{
			*parseError = true;
			fprintf(stderr, "ADDCON nécessite un paramètre (nombre entier).\n");
		}
	}
	else if(strcmp("DIMCON", argv[index]) == 0){
		*function = DIMCON;
		if(index < argc-1 && isdigit(argv[++index][0]) ){
			*functionValue = atoi(argv[index]);
		}
		//Sinon il y a une erreur (DIMCON a besoin d'une valeur)
		else{
			*parseError = true;
			fprintf(stderr, "DIMCON nécessite un paramètre (nombre entier).\n");
		}
	}
	else if(strcmp("INVERT", argv[index]) == 0){
		*function = INVERT;
	}
	else if(strcmp("SEPIA", argv[index]) == 0){
		*function = SEPIA;
	}
	else{ 
		fprintf(stderr,"Code lUT inconnu : %s.\n", argv[index]);
		*parseError = true;
	}
	
	if(*parseError){
		printf("Erreur dans la ligne de commande autour du mot %s\n", argv[index-1]);
	}
	
	index++;
	return index;
}

void displayCurrentLayer(List* layerList){
	if(layerList == NULL || layerList->type != LAYER)
		return;
	displayImage(currentLayer(layerList)->imgFinale);	
}

void displayImage(const Image* img){
	if(img == NULL || img->arrayRGB == NULL) return;
	
	actualiseImage(img->arrayRGB);
	
}

//TO DO
void printState(){
	extern List* layerList;
	Layer* lay = currentLayer(layerList);
	Lut* lt = NULL;
	
	//Ne doit pas arriver
	if(lay == NULL){
		fprintf(stderr, "Erreur : Aucun calque courant.\n");
		return;
	} 
	
	printf("\n----Position actuelle----\n");
	printf("Calque n°%d (id %d)\n", layerList->position, lay->id);
	printf("Image source : ");
	if(lay->imgSource->name != NULL)
		printf("%s\n", lay->imgSource->name);
	else
		printf(" Calque Vierge\n");
	printf("Opacité : %lf\n", lay->opacity);
	printf("Opération : ");
	if(lay->operation == SUM) printf("Somme\n");
	else printf("Multiplication\n");
	
	if(lay->lutList->size == 1) return;
	
	printf("Effets appliqués : \n");
	//On sauvegarde l'état de la liste des Lut
	ListState* savedState = saveListState(lay->lutList);
	goToHeadList(lay->lutList);
	
	while( ( lt = (Lut*)nextData(lay->lutList) ) != NULL){
		switch(lt->function){
			case INVERT :
				printf("\tInversion\n");
			break;
			case ADDLUM :
				printf("\tAjout de luminosité : %d\n", lt->valueEffect);
			break;
			case DIMLUM :
				printf("\tDiminution de luminosité : %d\n", lt->valueEffect);
			break;
			case ADDCON :
				printf("\tAjout de constraste : %d\n",lt->valueEffect);
			break;
			case DIMCON :
				printf("\tDiminution de contraste : %d\n",lt->valueEffect);
			break;
			case SEPIA :
				printf("\tSépia\n");
			break;
			case NEUTRAL : continue;
			break;
		}
	}
	restoreListState(savedState);
	free(savedState);	
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
		bool review = recordLayerOperation(layerList, newLay, CAL1);
		printf("L'ajout du calque a réussi.\n");
		displayCurrentLayer(layerList);
		
		if(!review) 
			fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");
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
	//+1 car sinon fgets s'arrête à maxNameLenght-1
	readStdin(fileChosen + strlen("./images/"), maxNameLenght+1);
	
		//Chargement de l'image
	img = loadImage(fileChosen);
	
	//Ne sert plus
	free(fileChosen);
	if(img == NULL){
		fprintf(stderr, "Erreur lors du chargement de l'image.");
		fprintf(stderr, " Création du calque annulée.\n");
		return;
	} 
	
	//Choix opacité
	opacity = userSetOpacity();
	
	//Choix opération
	operation = userSetLayerOp();
	
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
		bool review = recordLayerOperation(layerList, newLay, CAL1);
		printf("L'ajout du calque a réussi.\n");
		displayCurrentLayer(layerList);
		
		if(!review) 
			fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");
	}
	else{
		printf("L'ajout du calque a échoué.\n");
		free(img);
		free(newLay);
	}
	
}

double userSetOpacity(){
	double opacity;
	char buf[20]; //On vise large
	
	
	printf("\nDéfinissez l'opacité du calque (entre 0 et 1 compris) : ");
	readStdin(buf, 20);
	opacity = atof(buf);
	//scanf("%lf", &opacity);
	while(!isdigit(buf[0]) || (opacity = atof(buf) ) > 1.0 || opacity < 0.0){
		fprintf(stderr, "Opacité rentrée non comprise entre 0 et 1");
		printf("\nDéfinissez l'opacité du calque (entre 0 et 1 compris) : ");
		readStdin(buf,20);
	}
	
	
	return opacity;
}

LAYER_OP userSetLayerOp(){
	int opNum;
	LAYER_OP operation;
	char buf[2];
	
	printf("\nChoisissez l'opération de mélange du calque (entrez le nombre correspondant) : \n");
	printf("%d.Somme\n%d.Multiplication\n=>", SUM, MULTIPLICATION);
	readStdin(buf,2);
	while( !isdigit(buf[0]) ||( (opNum = atoi(buf) ) != SUM && opNum != MULTIPLICATION) ){
		printf("\nErreur de frappe.\nChoisissez l'opération de mélange du calque (entrez le nombre correspondant) : \n");
		printf("%d.Somme\n%d.Multiplication\n=>", SUM, MULTIPLICATION);
		readStdin(buf, 2);
	}
	
	operation = opNum;
	return operation;
}

bool userDelCurrentLayer(List* layerList){
	
	return delCurrentLayer(layerList);
}

void userSaveFinalImage(List* layerList){
	char imgName[51] = "./images/";
	imgName[51] = '\0';
	printf("Sauvegarde de l'image finale.\n");
	printf("Entrez le nom de l'image que vous voulez sauvegarder (max 50 caractères) : \n");
	
	readStdin(imgName + 9, 51);
		
	if(finalImage == NULL){
		generateFinalImage(layerList, &finalImage);
	}

	if (! imgAddName(finalImage, imgName) ){
		fprintf(stderr, "Le nommage de l'image a échoué. Nouveau nom : noname.ppm\n");
	}
	
	if( ! saveImage(finalImage) ){
		fprintf(stderr, "Une erreur est survenue pendant la sauvegarde de l'image finale.\n");
	}
	else{
		printf("%s sauvegardée.\n", imgName);
	}
	
}

bool userAddLut(List* layerList){
	if(layerList == NULL) return false;
	
	char choice[2], value[4];
	char numChoice = 0, numValue = 0;
	Layer* lay = currentLayer(layerList);
	
	LUT_FUNCTION lF = NEUTRAL;
	
	puts("Effet à ajouter au calque courant :");
	printf("%d.Invert\n", INVERT);
	printf("%d.Augmenter luminosité\n", ADDLUM);
	printf("%d.Diminuer luminosité\n", DIMLUM);
	printf("%d.Augmenter le contraste\n", ADDCON);
	printf("%d.Diminuer le constraste\n", DIMCON);
	printf("%d.Effet sépia\n", SEPIA);
	printf("Entrez le numéro correspondant : ");
	readStdin(choice, 2);
	
	numChoice = atoi(choice);
	
	lF = numChoice;
	
	//Si une valeur doit être demandée
	if(lF != INVERT && lF != SEPIA){
		printf("Valeur de l'effet : ");
		readStdin(value, 4);
		numValue = atoi(value);
	}
	
	//Création du Lut et ajout de ce lut dans la liste.
	//Tout est géré par layer.c:addLut(...)
	if(addLut(lay, lF, numValue)){
		goToBottomCell(lay->lutList);//Sécurité
		Lut* newLut = (Lut*) currentData(lay->lutList);
		if( !recordLutOperation(lay->lutList, lay, newLut, LUT1) ){
			fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");
		}
		return true;
	}
	return false;
}

bool userDelCurrentLut(List* layerList){
	if(layerList == NULL) return false;
	
	Layer* lay = currentLayer(layerList);
	
	//Cette fonction se charge de recalculer l'image finale
	Lut* lutToDelete = delLastLut(lay);
	
	if(lutToDelete == NULL){
		printf("\nAucun effet à supprimer sur le calque courant !\n");
		return false;
	}
	
	//Il faut sauvegarder le lut avant de le supprimer
	if( !recordLutOperation(lay->lutList, lay,lutToDelete, LUT3) ){
		fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");
	}
	
	freeLut(lutToDelete);
	return true;
}

//A FINIR
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
			if( !recordLayerOperation(layerList, currentLayer(layerList), CAL3)){
				fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");	
			}
			setLayerOpacity(currentLayer(layerList), opacity);
			printf("Opacité du calque courant modifiée à %lf\n", opacity);
		break;
		case 'm' :
			operation = userSetLayerOp();
			if( !recordLayerOperation(layerList, currentLayer(layerList), CAL4)){
				fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");	
			}
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
				if( !recordLayerOperation(layerList, currentLayer(layerList), CAL5)){
					fprintf(stderr, "Une erreur est survenue lors de l'ajout de la dernière opération dans l'historique.\n");	
				}
				
			}
			else
				printf("\n\t /!\\Un seul calque est présent, vous ne pouvez pas le supprimer./!\\\n");
			printState();
		break;
		case 's':
			userSaveFinalImage(layerList);
		break;
		case ' ' :
			displayCommands();
		break;
		case 'h' :
			displayReview();
		break;
		case 'u' :
			undo();
			displayCurrentLayer(layerList);
			printState();
		break;
		case 'l' :
			//Historique géré dans la fonction
			userAddLut(layerList);
			displayCurrentLayer(layerList);
			printState();
		break;
		case 'n':
			if(userDelCurrentLut(layerList))
				printf("\n\t---->Dernier effet supprimé.\n");
			
			displayCurrentLayer(layerList);
			printState();
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
			displayCurrentLayer(layerList);
			printState();
		break;
		case GLUT_KEY_RIGHT :
			lay = nextLayer(layerList);
			//Si on était sur le dernier layer, lay == NULL
			if(lay == NULL) return;
			displayCurrentLayer(layerList);
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
	//Suppression de l'historique
	freeReview();
}

void displayCommands(){
 	printf("\n\n****** Commandes utilisateurs ******\n\n");
	puts("KEY_LEFT : calque précédent");
	puts("KEY_RIGHT : calque suivant");
	puts("ESCAPE : quitter le programme");
	puts("SPACE : afficher les commandes");
	puts("a: ajouter un calque");
	puts("g: générer image finale");
	puts("c: afficher le layer courant (utile si on est sur l'image finale)");
	puts("v: ajouter un calque vierge");
	puts("o: modifier l'opacité du calque courant");
	puts("m: modifier la fonction de mélange du calque courant");
	puts("d: supprimer le calque courant");
	puts("s: sauvegarder l'image finale");
	puts("h: afficher l'historique");
	puts("u: annuler la dernière opération");
	puts("l: ajouter un effet au calque courant");
	puts("n: supprimer l'effet courant (le dernier de la liste du calque courant");
}















