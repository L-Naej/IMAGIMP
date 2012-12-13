#include "list.h"
#include "layer.h"//Pour dumpLayer
#include <stdlib.h>

Cell* createCell(void* userData){
	Cell* c = (Cell*) malloc(sizeof(Cell));
	if(c == NULL) return NULL;
	
	c->userData = userData;
	c->previous = NULL;
	c->next = NULL;
	
	return c;
}

List* createEmptyList(LIST_TYPE type){
	List* list = (List*) malloc(sizeof(List));
	if(list == NULL) return NULL;
	
	list->head = list->cursor = list->bottom = NULL;
	list->size = 0;
	list->type = type;
	list->position = 0;
	
	return list;
}

List* createList(LIST_TYPE type, void* headUserData){
	List* list = createEmptyList(type);
	if(list == NULL || headUserData == NULL) return NULL;
	
	list->head = createCell(headUserData);
	list->bottom = list->head;
	if(list->head == NULL){
		free(list);
		return NULL;
	}
	
	list->head->previous = NULL;
	
	//On place le curseur en début de liste (avant head)
	list->cursor = NULL;	
	list->size = 1;
	list->position = 0;
	return list;
}

Cell* currentCell(List* list){
	if(list == NULL) return NULL;
	return list->cursor;
}

//Note: si curseur en début de list cursor == NULL
//	  si curseur en fin de liste, cursor->next == NULL
Cell* nextCell(List* list){
	if(list == NULL) return NULL;
	
	//Cas spécial : curseur en début de liste, le curseur avance donc sur head
	if(list->cursor == NULL){
		list->cursor = list->head;
		list->position++;
		return list->cursor;
	}
	
	//Cas spécial : curseur en fin de liste, on ne bouge pas le curseur
	if(list->cursor->next == NULL) return NULL;
	
	//Cas général, on avance le curseur et on le retourne
	list->cursor = list->cursor->next;
	list->position++;
	
	return list->cursor;
}

//Note: si curseur en début de list cursor == NULL
//	  si curseur en fin de liste, cursor->next == NULL
Cell* previousCell(List* list){
	if(list == NULL) return NULL;
	
	//Cas spécial : curseur en début de liste
	if(list->cursor == NULL){
		return NULL;
	}
	
	list->cursor = list->cursor->previous;
	list->position--;
	
	return list->cursor;
}

void* nextData(List* list){
	Cell* c = nextCell(list);
	if(c) return c->userData;
	return NULL;
}

void* previousData(List* list){
	Cell* c = previousCell(list);
	if(c) return c->userData;
	return NULL;
}

void* currentData(List* list){
	Cell* c = currentCell(list);
	if(c) return c->userData;
	return NULL;
}

int listCountElem(List* list){
	int size = 0;
	if(list == NULL) return size;
	
	ListState* state = saveListState(list);
	goToHeadList(list);
	while(nextCell(list) != NULL) size++;
	
	restoreListState(state);
	free(state);
	
	return size;
}

bool isListEmpty(List* list){
	if(list == NULL) return true;
	
	if(list->size <= 0) return true;
	return false;
}

Cell* insertHeadCell(List* list, void* userData){
	//La liste doit être initialisée (sinon ça devient le bordel)
	if(list == NULL || userData == NULL){
		fprintf(stderr,"Tentative d'insertion d'un élement dans une liste nulle.\n");	
		return NULL;
	} 
	Cell* c = createCell(userData);
	if(c == NULL){
		fprintf(stderr, "Erreur lors de l'ajout d'une cellule en tête de liste.\n");
		return NULL;	
	} 
	
	c->previous = NULL;
	c->next = list->head;
	list->head->previous = c;
	list->head = c;
	
	list->size++;
	
	return c;
}

Cell* insertBottomCell(List* list, void* userData){
	//La liste doit être initialisée (sinon ça devient le bordel)
	if(list == NULL || userData == NULL){
		fprintf(stderr,"Tentative d'insertion d'un élement dans une liste nulle.\n");	
		return NULL;
	} 
	
	Cell* c = createCell(userData);
	if(c == NULL){
		fprintf(stderr, "Erreur lors de l'ajout d'une cellule en fin de liste.\n");
		return NULL;	
	}
	 
	c->previous = list->bottom;
	list->bottom->next = c;
	c->next = NULL;
	list->bottom = c;
	
	list->size++;
	return c;
}

Cell* insertAfterCell(List* list, void* userData){
	if(list == NULL || userData == NULL) return NULL;
	
	Cell* cToInsert = createCell(userData);
	if(cToInsert == NULL){
		fprintf(stderr, "Erreur lors de l'ajout d'une cellule dans une liste");
		return NULL;
	}
	
	//-------On relie cToInsert à son voisin de droite
	
	//Cas spécial : on a inséré après la tête de liste
	if(list->cursor == NULL){
		cToInsert->next = list->head;
	}
	else{
		cToInsert->next = list->cursor->next;
	}
	
	//Cas spécial : on a inséré après le dernier élément
	if(list->cursor == list->bottom){
		cToInsert->next = NULL;
		list->bottom = cToInsert;
	}
	else{
		cToInsert->next->previous = cToInsert;
	}
	
	//On relie cToInsert à son voisin de gauche
	cToInsert->previous = list->cursor;
	
	
	//----------Cas spécial : on a inséré après la tête de liste 
	//(donc pas de voisin de gauche)
	if(list->cursor == NULL){
		//Nouvelle tête de liste
		list->head = cToInsert;
	}
	else{
		list->cursor->next = cToInsert;
	}
	
	list->size++;
	
	return cToInsert;
}

Cell* insertBeforeCell(List* list, void* userData){
	if(list == NULL || userData == NULL) return NULL;
	
	Cell* cToInsert = createCell(userData);
	if(cToInsert == NULL){
		fprintf(stderr, "Erreur lors de l'ajout d'une cellule dans une liste");
		return NULL;
	}
	//On relie cToInsert à son voisin de gauche
	
	//Cas spécial : on est au niveau de la tête OU en tête de liste
	if(list->cursor == NULL || list->cursor == list->head){
		cToInsert->previous = NULL;
	}
	else{
		cToInsert->previous = list->cursor->previous;
		cToInsert->previous->next = cToInsert;
	}
	
	//On relie cToInsert à son voisin de droite
	//Cas spécial: on a inséré avant la tête OU en tête de liste
	if(list->cursor == NULL || list->cursor == list->head){
		cToInsert->next = list->head;
		list->head->previous = cToInsert;
		list->head = cToInsert;
	}
	else{
		cToInsert->next = list->cursor;
		list->cursor->previous = cToInsert;
	}

	
	list->size++;
	
	return cToInsert;
}

void goToHeadList(List* list){
	if(list == NULL) return;
	
	list->cursor = NULL;
	list->position = 0;	
}

Cell* goToBottomCell(List* list){
	if(list == NULL) return NULL;
	
	list->cursor = list->bottom;
	list->position = list->size;
	
	return list->cursor;
}

Cell* getCellByPosition(List* list, int position){
	if(list == NULL || position <= 0 || position > list->size)
		return NULL;
	int i;
	
	//Cas spéciaux
	if(position == 1) return list->head;
	if(position == list->size) return list->bottom;
	
	//Optimisation, woulala !
	//Si la position est plus proche de la fin on part de la fin,
	//et vice-versa.
	if(position > (list->size / 2 )){
		goToBottomCell(list);
		for(i = list->size; i >position; --i){
			previousCell(list);
		}
	}
	else{
		goToHeadList(list);
		for(i = 0; i < position; ++i){
			nextCell(list);
		}
	}
	
	return list->cursor;
}

void* delCellByPosition(List* list, int position){
	if(list == NULL || position < 0 || position > list->size)
		return NULL;
		
	Cell* theCell = getCellByPosition(list, position);
	
	//size-- est fait dans delCellInList
	return delCellInList(list, theCell);
}

void* delCellInList(List* list, Cell* theCell){
	if(list == NULL || theCell == NULL) return NULL;
	void* userData = NULL;
	
	//On coupe les liens !
	
	//Cas spécial : c'est le dernier cell
	if(list->size == 1){
		list->head = list->bottom = list->cursor = NULL;
		list->position = 0;
		list->size--;
	}
	//Cas spécial : le Cell est en fin de liste
	else if(theCell == list->bottom){
		theCell->previous->next = NULL;
		list->bottom = theCell->previous;
		theCell->previous = NULL;
		list->size--;
		//Si le curseur était sur theCell..
		if(list->cursor == theCell){
			printf("pret %p et %p\n", list->cursor, theCell);
			list->cursor = list->bottom;
			list->position = list->size;
		}

	}
	//Cas spécial : le Cell est en tête de liste
	else if(theCell == list->head){
		theCell->next->previous = NULL;
		list->head = theCell->next;
		theCell->next = NULL;
		list->size--;
		//Si le curseur était sur theCell..
		if(list->cursor == theCell){
			list->cursor = list->head;
			list->position = 1;
		}
	}
	//Sinon cas général
	else{
		theCell->next->previous = theCell->previous;
		theCell->previous->next = theCell->next;
	
		//Si le curseur était sur theCell..
		if(list->cursor == theCell){
			list->cursor = theCell->next;
		}
		list->size--;
		theCell->next = NULL;
		theCell->previous = NULL;
	}	
	userData = theCell->userData;
	free(theCell);
	
	return userData;
}

void freeCellByPosition(List* list, int position){
	//delCellByPosition fait les tests pour nous...
	
	void* userData = delCellByPosition(list,position);
	
	switch(list->type){
		case LAYER : freeLayer((Layer*)userData);
		break;
		case LUT : freeLut((Lut*)userData);
		break;
		case UNKNOWN : free(userData);
		break;
		default : free(userData);
		break;
	}
}

void freeCellInList(List* list, Cell* c){
	void* userData = delCellInList(list,c);
	switch(list->type){
		case LAYER : freeLayer((Layer*)userData);
		break;
		case LUT : freeLut((Lut*)userData);
		break;
		case UNKNOWN : free(userData);
		break;
		default : free(userData);
		break;
	}
}

void freeCell(Cell* c){
	free(c);
}


//Comme on supprime les cellules, le curseur se déplace tout
//seul. Il ne faut donc pas travailler sur nextCell mais sur
//currentCell.
void freeList(List* list){
	if(list == NULL) return;
	Cell* c = NULL;
	
	goToHeadList(list);
	
	while( (c = nextCell(list) ) != NULL){
		freeCellInList(list, c);
	}
	
	free(list);
}

//Comme on supprime les cellules, le curseur se déplace tout
//seul. Il ne faut donc pas travailler sur nextCell mais sur
//currentCell.
void freeListComplete(List* list){
	if(list == NULL) return;
	
	Cell* c = NULL;
	
	goToHeadList(list);
	nextCell(list);
	c = currentCell(list);
	while( c != NULL){
		freeCellInList(list, c);
		c = currentCell(list);
	}
	
	free(list);
}

void dumpList(List* list){
	if(list == NULL) return;
	int cnt = 1;
	
	printf("\n--------------Affichage Liste (Adresse : %p)--------------\n\n", list);
	if(isListEmpty(list)){
		printf("\tListe vide.\n");
		return;
	}
	
	ListState* state = saveListState(list);
	goToHeadList(list);

	
	while(nextCell(list) != NULL){
		printf("\n\tCellule %d)  Adresse : %p", cnt, currentCell(list));
		switch(list->type){
			case LAYER : dumpLayer((Layer*)list->cursor->userData);
			break;
			case LUT: printf("de type LUT\n");
			break;
			case UNKNOWN : printf("de type inconnu\n");
			 break;
		}
		if(list->cursor->next != NULL)
			printf("Next Cell : %p\n", list->cursor->next);
		else
			printf("Next Cell : None\n");
		if(list->cursor->previous != NULL)
			printf("Previous Cell: %p\n", list->cursor->previous);	
		else
			printf("Previous Cell : None\n");
		cnt++;
	}
	printf("\n----->Fin Affichage Liste (Adresse : %p)\n", list);
	
	restoreListState(state);
	free(state);
}

ListState* saveListState(List* list){
	if(list == NULL) return NULL;
	
	ListState* state = (ListState*) malloc(sizeof(ListState));
	if(state == NULL) return NULL;
	state->savedList = list;
	state->savedCell = currentCell(list);
	state->savedPosition = list->position;
	
	return state;
}

void restoreListState(ListState* state){
	if(state == NULL || state->savedList == NULL)
		return;
	
	Cell* curCell = NULL;
	
	goToHeadList(state->savedList);
	//Cas spécial : on était en tête (curseur à NULL), rien à faire !
	if(state->savedCell == NULL){
		return;
	}
	//SINON
	//On vérifie que la Cell sauvegardée n'est pas morte
	//entre temps en allant à son ancienne position et en checkant
	//qu'elle est toujours là.
	curCell = nextCell(state->savedList);
	while( curCell != NULL ){
		//Si on trouve la Cell, on peut partir on est désormais dessus !
		if(curCell == state->savedCell){
			return;
		}
			
		curCell = nextCell(state->savedList);
	}
	

	//Si la cell existe encore, ou bien qu'elle n'existe plus
	//mais que  son ancienne position dépasse
	//la taille actuelle de la liste, on s'arrête là (dans le
	//2eme cas le curseur est donc à la fin).
	if(curCell != NULL 
	|| state->savedList->size < state->savedPosition) return;
	
	//Si la Cell n'existe plus (curCell == NULL), on se positionne
	// à son ancienne...position
	goToHeadList(state->savedList);
	while(state->savedList->position != state->savedPosition ){
		nextCell(state->savedList);
	}
}
