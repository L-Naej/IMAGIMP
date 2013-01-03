#include "review.h"
#include "layersManager.h"
#include "list.h"
#include "ihm.h"//Pour displayCurrentLayer voir fonction undo

//Variable globale représentant l'historique
List* review = NULL;

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
	
	lastOp = popOperation();
	if(lastOp == NULL) return;
	
	switch(lastOp->name){
		//On ne peut pas annuler IM1
		case IM1 :
			//On remet l'opération dans l'historique
			pushOperation(lastOp);
			printf("Annulation du chargement de la première image impossible.\n");
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
		//On ne peut pas annuler CAL5
		case CAL5:
			//On remet l'opération dans l'historique
			pushOperation(lastOp);
			printf("Annulation de la suppression d'un calque impossible.\n");
		break;
		default : printf("Dernière opération à annuler inconnue.\n");
		break;
	}
	
	freeOperation(lastOp);
	
}

bool recordLayerOperation(List* layerList, Layer* lay, OperationName opName){
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
	
	ReviewType type;
	type.savedLayer = sL;
	
	Operation* op = createOperation(opName, type, layerList);
	
	pushOperation(op);
	return true;
}

bool recordImgOperation(Image* img, OperationName opName){
	if(img == NULL || opName != IM1) return false;

	ReviewType type;
	type.img = img;
	
	Operation* op = createOperation(opName, type, NULL);
	
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

void displayReview(){
	goToBottomCell(review);
	Operation* curOp = NULL;
	
	printf("\n\n----------HISTORIQUE DES OPERATIONS (ordre chronologique inverse)----------\n");
	curOp = (Operation*) currentData(review);
	printf("\n\tOperation : ");
		switch(curOp->name){
			case IM1 : 
				printf("Chargement de l'image de base.\n");
				printf("Image chargée : %s\n", curOp->type.img->name);
				printf("\n///////////////////////////////////////\n");
			break;
			case CAL1 : 
				printf("Ajout d'un calque.\n");
				printf("Calque ajouté en position %d\n", curOp->type.savedLayer->position);
			break;
			case CAL3 : 
				printf("Changement d'opacité d'un calque.\n");
				printf("Calque n°%d\n", curOp->type.savedLayer->position);
				printf("Ancienne opacité : %lf\n", curOp->type.savedLayer->oldOpacity);
			break;
			case CAL4 : 
				printf("CAL4");
				printf("Changement d'opération d'un calque.\n");
				printf("Calque n°%d\n", curOp->type.savedLayer->position);
				printf("Ancienne opération : ");
				if(curOp->type.savedLayer->oldOperation == SUM) printf("Somme\n");
				else printf("Multiplication\n");
			break;
			case CAL5 : 
				printf("Suppression du calque n°%d", curOp->type.savedLayer->position);
				printf("\n///////////////////////////////////////\n");
			break;
			default : printf("Opération inconnue.\n");
		}
			
	while( ( curOp = (Operation*) previousData(review) ) != NULL){
		printf("\n\tOperation : ");
		switch(curOp->name){
			case IM1 : 
				printf("Chargement de l'image de base.\n");
				printf("Image chargée : %s\n", curOp->type.img->name);
				printf("\n///////////////////////////////////////\n");
			break;
			case CAL1 : 
				printf("Ajout d'un calque.\n");
				printf("Calque ajouté en position %d\n", curOp->type.savedLayer->position);
			break;
			case CAL3 : 
				printf("Changement d'opacité d'un calque.\n");
				printf("Calque n°%d\n", curOp->type.savedLayer->position);
				printf("Ancienne opacité : %lf\n", curOp->type.savedLayer->oldOpacity);
			break;
			case CAL4 : 
				printf("CAL4");
				printf("Changement d'opération d'un calque.\n");
				printf("Calque n°%d\n", curOp->type.savedLayer->position);
				printf("Ancienne opération : ");
				if(curOp->type.savedLayer->oldOperation == SUM) printf("Somme\n");
				else printf("Multiplication\n");
			break;
			case CAL5 : 
				printf("Suppression du calque n°%d", curOp->type.savedLayer->position);
				printf("\n///////////////////////////////////////\n");
			break;
			default : printf("Opération inconnue.\n");
			break;
		}
		
	}
	
		
}

void freeOperation(Operation* op){
	if(op == NULL) return;
	switch(op->name){
		case CAL1 :
		case CAL3 :
		case CAL4 :
		case CAL5 :
			free(op->type.savedLayer);
		break;
	}
	free(op);
}

void freeReview(){
	freeListComplete(review);
}

