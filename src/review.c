#include "review.h"
#include "layersManager.h"
#include "list.h"
#include "ihm.h"//Pour displayCurrentLayer voir fonction undo
#include "image.h"
#include <string.h>

//Variable globale représentant l'historique
List* review = NULL;

//Fonctions internes
void printSavedLut(SavedLut* sL);
Layer* regenerateLayer(SavedLayer* sL);
void restoreLayerInList(List* layerList, Layer* lay, int position, bool atEnd);
SavedLut* saveLuts(List* lutList);

void initReview(){
	//Si un historique existe déjà, 
	//cette fonction le détruit.
	if(review != NULL){
		freeReview(review);
		review = NULL;
	}		
	
	review = createEmptyList(REVIEW);
}

void pushOperation(Operation* op){
	if(op == NULL) return;
	if(review == NULL)
		initReview();
		
	insertBottomCell(review, op);
}

Operation* popOperation(){
	if(review == NULL) return NULL;
	Operation* lastOp = NULL;
	
	goToBottomCell(review);
	
	//On récupère la dernière operation
	lastOp = (Operation*) currentData(review);
	
	//Puis on la supprime de la liste(pile) sans 
	//nettoyer la mémoire puisqu'on en a besoin après
	delCellInList(review, currentCell(review));
	
	return lastOp;
}

void undo(){
	Operation* lastOp = NULL;
	Layer* rL = NULL; //Si undo CAL5
	lastOp = popOperation();
	if(lastOp == NULL) return;
	
	printf("\n\n/!\\ Annulation /!\\\n\t");
	switch(lastOp->name){
		//On ne peut pas annuler IM1
		case IM1 :
			//On remet l'opération dans l'historique
			pushOperation(lastOp);
			printf("Annulation du chargement de la première image impossible.\n");
			return;//Impératif sinon l'opération est supprimée !!!
		break;
		case CAL1 : 
			//A faire tout de suite sinon opengl risque 
			//de tenter d'afficher une image d'un layer qui n'existe plus
			if(currentCell(lastOp->list) != lastOp->list->head){
				previousCell(lastOp->list);
				displayCurrentLayer(lastOp->list);
				nextCell(lastOp->list);
			}
			delLayer(lastOp->list, lastOp->type.savedLayer->ptrLay);
			printf("Le dernier layer ajouté a été supprimé.\n");
		break;
		case CAL3 : 
			setLayerOpacity( lastOp->type.savedLayer->ptrLay, lastOp->type.savedLayer->oldOpacity);
			printf("L'ancienne opacité du calque a été restaurée.\n");
		break;
		case CAL4:
			setLayerOperation(lastOp->type.savedLayer->ptrLay, lastOp->type.savedLayer->oldOperation);
			printf("L'ancienne opération du calque a été restaurée.\n");
		break;
		//Appel à des fonctions internes pour ne pas surcharger le switch
		case CAL5:
			rL = regenerateLayer(lastOp->type.savedLayer);
			if(rL == NULL){
				fprintf(stderr, "Impossible d'annuler la suppression du calque.\n");
				pushOperation(lastOp);
				return;//Impératif sinon l'opération est supprimée !!!
			}
			
			restoreLayerInList(lastOp->list, rL, lastOp->type.savedLayer->position, lastOp->type.savedLayer->atEnd);
			printf("Le layer d'id %d d'image source %s a été régénéré à son ancienne position.\n", rL->id, rL->imgSource->name);
		break;
		case LUT1:
			delLastLut(lastOp->type.savedLut->owner);
			printf("Le dernier effet ajouté à été supprimé.\n\t");
			printSavedLut(lastOp->type.savedLut);
		break;
		case LUT3:
			//On régénère le lut supprimé
			if(!addLut(lastOp->type.savedLut->owner, lastOp->type.savedLut->function, lastOp->type.savedLut->functionValue)){
				fprintf(stderr, "\nErreur : Impossible d'annuler la suppression de l'effet.\n");
				return;//Impératif sinon l'opération est supprimée !!!
			}
			printf("La suppression du Lut a été annulée.\n\t");
			printSavedLut(lastOp->type.savedLut);
		break;
		default : printf("Dernière opération à annuler inconnue.\n");
		break;
	}
	
	freeOperation(lastOp);
	
}

/*
 * Recette :
 * 1. Recharger en mémoire l'image source
 * 2. Appeler createLayer avec les données de sL
 * 3. Redonner son id au layer
 * 4. Recharger la liste de luts
 * 4. C'est prêt !
 */
Layer* regenerateLayer(SavedLayer* sL){
	if(sL == NULL)
		return NULL;
	Layer* rL = NULL;
	Image* source = NULL;
	int i = 0;
	 
	 if(sL->imgName == NULL || strcmp(sL->imgName, "(calque vierge)") == 0){
	 	source = createEmptyImg(sL->width, sL->height, 255);
	 	source->name = "(calque vierge)";
	 }
	 else source = loadImage(sL->imgName);
	 if(source == NULL){
	 	fprintf(stderr, "Impossible de régénérer l'image du calque. Annulation de la suppression impossible (erreur allocation mémoire).\n");
	 	return NULL;
	 }
	 
	 rL = createLayer(source, sL->oldOpacity, sL->oldOperation);
	 if(rL == NULL){
	 	fprintf(stderr, "Impossible d'allouer la mémoire pour régénérer le calque.\n");
	 	freeImage(source);
	 	return NULL;
	 }
	 //On efface le nouvel id pour donner l'ancien (du coup : saut dans les id)
	 rL->id = sL->id;
	 
	 
	 for(i = 0; i < sL->nbLuts; ++i){
	 	addLut(rL, sL->lstLuts[i].function, sL->lstLuts[i].functionValue);
	 }
	 
	 return rL;
}

/*
 * Recette cas général:
 * 1. Sauvegarder l'état de la liste
 * 2. Positionner le curseur de la liste à l'ancienne position
 *    du calque.
 * 3. Insérer le calque AVANT le curseur.
 * 4. Restaurer l'état de la liste (et le supprimer de la mémoire).
 * 
 * Cas spéciaux : calque en fin de liste.
 */
void restoreLayerInList(List* layerList, Layer* lay, int position, bool atEnd){
	if(layerList == NULL || lay == NULL) return;
	
	ListState* state = saveListState(layerList);
	
	//Si le layer était en fin de liste
	if(atEnd){
		insertBottomCell(layerList,lay);
	}
	
	//Cas général (début de liste inclus)
	else {
		goToPosition(layerList, position);
		insertBeforeCell(layerList, lay);
	}

	restoreListState(state);
	
	free(state);
}

SavedLut* saveLuts(List* lutList){
	if(lutList == NULL) return NULL;
	Lut* l = NULL;
	int i = 0;
	SavedLut* luts = (SavedLut*) calloc(lutList->size, sizeof(SavedLut));
	if(luts == NULL){
		fprintf(stderr, "Impossible de stocker dans l'historique la liste des effets du calque\n");
		return NULL;
	}
	
	ListState* state = saveListState(lutList);
	goToHeadList(lutList);
	nextData(lutList);//On ne compte pas neutral
	
	while( (l = (Lut*)nextData(lutList) ) != NULL ) {
		luts[i].function = l->function;
		luts[i].functionValue = l->valueEffect;
		i++;
	}
	
	restoreListState(state);
	free(state);
	return luts;
}

bool recordLayerOperation(List* layerList, Layer* lay, OperationName opName, bool atEnd){
	if(layerList == NULL || lay == NULL)
		return false;
	
	//L'opération doit être une opération de calque
	switch(opName){
		case CAL1 :
		case CAL3 :
		case CAL4 :
		case CAL5 :
		break;
		default : return false;
		break;
	}
	
	//Sauvegarde de l'état du layer
	SavedLayer* sL = (SavedLayer*) malloc(sizeof(SavedLayer));
	if(sL == NULL){
		fprintf(stderr, "Impossible d'enregistrer l'opération dans l'historique.\n");
		return false;
	}
	
	sL->ptrLay = lay;
	sL->oldOpacity = lay->opacity;
	sL->oldOperation = lay->operation;
	sL->position = layerList->position;
	sL->id = lay->id;
	sL->imgName = NULL;
	sL->width = lay->imgSource->width;
	sL->height = lay->imgSource->height;
	sL->atEnd = atEnd;
	sL->lstLuts = NULL;
	sL->nbLuts = 0;
	
	if(opName == CAL5){
		sL->lstLuts = saveLuts(lay->lutList);
		sL->nbLuts = lay->lutList->size - 1;//On ne compte pas le neutral
	}
	
	if(lay->imgSource->name != NULL){
		sL->imgName = (char*) calloc(strlen(lay->imgSource->name)+1, sizeof(char));
		if(sL->imgName != NULL){
			strcpy(sL->imgName, lay->imgSource->name);
		}
	}
	
	ReviewType type;
	type.savedLayer = sL;
	
	Operation* op = createOperation(opName, type, layerList);
	if(op == NULL){
		fprintf(stderr, "Impossible d'enregistrer l'opération dans l'historique.\n");
		free(sL);
		return false;		
	}
	pushOperation(op);
	return true;
}

bool recordImgOperation(Image* img, OperationName opName){
	if(img == NULL || opName != IM1) return false;

	ReviewType type;
	if(img->name == NULL) type.imgName = "Nom de l'image inconnu";
	else{
		type.imgName = (char*) calloc(strlen(img->name)+1, sizeof(char));
		if(type.imgName == NULL){
			fprintf(stderr, "Impossible de sauvegarder le nom de l'image initiale (IM1) dans l'historique.\n");
		}
		else{
			strcpy(type.imgName, img->name);
		}
	}
	
	
	Operation* op = createOperation(opName, type, NULL);
	if(op == NULL){
		fprintf(stderr, "Impossible d'enregistrer l'opération dans l'historique.\n");
		return false;		
	}
	
	pushOperation(op);
	return true;
}

bool recordLutOperation(List* lutList, Layer* owner, Lut* lt, OperationName opName){
	if(lutList == NULL || lt == NULL || owner == NULL || (opName != LUT1 && opName != LUT3) )
		return false;
		
	SavedLut* sL = (SavedLut*) calloc(1, sizeof(SavedLut));
	if(sL == NULL){
		fprintf(stderr, "Impossible d'enregistrer l'opération dans l'historique.\n");
		return false;
	}
	sL->owner = owner;
	sL->function = lt->function;
	sL->functionValue = lt->valueEffect;
	sL->maxValue = lt->maxValue;
	sL->ownerId = owner->id;
	
	if(owner->imgSource->name != NULL){
		sL->imgName = (char*) calloc(strlen(owner->imgSource->name)+1, sizeof(char));
		if(sL->imgName != NULL){
			strcpy(sL->imgName, owner->imgSource->name);
		}
	}
	
	ReviewType type;
	type.savedLut = sL;
	
	Operation* op = createOperation(opName, type, lutList);
	if(op == NULL){
		fprintf(stderr, "Impossible d'enregistrer l'opération dans l'historique.\n");
		free(sL);
		return false;		
	}
	
	pushOperation(op);
	return true;
}

Operation* createOperation(OperationName name, ReviewType type, List* list){
	Operation* op = (Operation*) malloc(sizeof(Operation));
	if(op == NULL) return NULL;
	
	op->name = name;
	op->type = type;
	op->list = list;
	
	return op;	
}

//Fonction interne
void printSavedLut(SavedLut* sL){
	switch(sL->function){
	case INVERT : 
		printf("Inversion\n");
	break;
	case ADDLUM : 
		printf("Ajout de luminosité : %d\n", sL->functionValue);
	break;
	case DIMLUM : 
		printf("Diminution de luminosité : %d\n", sL->functionValue);
	break;

	case ADDCON : 
		printf("Augmentation du contraste : %d\n", sL->functionValue);
	break;
	case DIMCON : 
		printf("Diminution du contraste : %d\n", sL->functionValue);
	break;
	case SEPIA :
		printf("Effet sépia\n");
	break;
	case GRAY :
		printf("Conversion de l'image en noirs et blancs\n");
	break;
	//Ne devrait pas arriver
	default : fprintf(stderr,"Fonction LUT inconnue.\n");
	break;
	}
	
	printf("\tSur le calque d'id n°%d\n", sL->ownerId);
	if(sL->imgName != NULL)
		printf("\tD'image source %s\n", sL->imgName);
}


void printOperation(Operation* curOp){
	if(curOp == NULL) return;
	printf("\n\tOperation : ");
	switch(curOp->name){
		case IM1 : 
			printf("Chargement de l'image de base.\n");
			if(curOp->type.imgName != NULL)
				printf("\tImage chargée : %s\n", curOp->type.imgName);
		break;
		case CAL1 : 
			printf("Ajout d'un calque.\n");
			printf("\tCalque ajouté en position %d\n", curOp->type.savedLayer->position);
			if(curOp->type.savedLayer->imgName != NULL)
				printf("\tD'image source : %s\n", curOp->type.savedLayer->imgName);
			else
				printf("\tCalque vierge (ou nom d'image source inconnu).\n");
		break;
		case CAL3 : 
			printf("Changement d'opacité d'un calque.\n");
			printf("\tCalque n°%d\n", curOp->type.savedLayer->position);
			printf("\tAncienne opacité : %lf\n", curOp->type.savedLayer->oldOpacity);
		break;
		case CAL4 : 
			printf("\tChangement d'opération d'un calque.\n");
			printf("\tCalque n°%d\n", curOp->type.savedLayer->position);
			printf("\tAncienne opération : ");
			if(curOp->type.savedLayer->oldOperation == SUM) printf("Somme\n");
			else printf("Multiplication\n");
		break;
		case CAL5 : 
			printf("Suppression du calque n°%d (d'id %d)\n", curOp->type.savedLayer->position, curOp->type.savedLayer->id);
			
		break;
		case LUT1 :
			printf("Ajout d'un effet\n\t");
			printSavedLut(curOp->type.savedLut);
		break;
		case LUT3 :
			printf("Suppression d'un effet\n\t");
			printSavedLut(curOp->type.savedLut);
		break;
		default : printf("Opération inconnue.\n");
		break;
	}
}

void displayReview(){
	goToHeadList(review);
	Operation* curOp = NULL;
	
	printf("\n\n----------HISTORIQUE DES OPERATIONS (ordre chronologique)----------\n");			
	while( ( curOp = (Operation*) nextData(review) ) != NULL){
		printOperation(curOp);
	}
	
		
}

void freeOperation(Operation* op){
	if(op == NULL) return;
	switch(op->name){
		case IM1 : 
			free(op->type.imgName);
		break;
		case CAL1 :
		case CAL3 :
		case CAL4 :
		case CAL5 :
			free(op->type.savedLayer->imgName);
			free(op->type.savedLayer->lstLuts);
			free(op->type.savedLayer);
		break;
		case LUT1:
		case LUT3:
			free(op->type.savedLut->imgName);
			free(op->type.savedLut);
		break;
		default : //Rien
		break;
	}
	free(op);
}

void freeReview(){
	freeListComplete(review);
}

