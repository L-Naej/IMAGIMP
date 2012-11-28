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
	
	return list;
}

List* createList(LIST_TYPE type, Cell* head){
	List* list = createEmptyList(type);
	if(list == NULL || head == NULL) return NULL;
	
	list->head = head;
	list->head->previous = NULL;
	
	//On place le curseur en début de liste (avant head)
	list->cursor = NULL;	
	
	//Vérifie que le Cell "head" n'est pas déjà connecté
	//à d'autres Cells, si oui, ils font donc partie de la liste
	//(ne devrait pas arriver)
	list->size = listCountElem(list);
	
	//On va en fin de liste (si le Cell passé
	//en paramètre est lié à d'autres, sinon list->head == list->bottom)
	while(nextCell(list) != NULL);
	list->bottom = list->cursor;
	
	list->cursor = NULL;
	
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
		return list->cursor;
	}
	
	//Cas spécial : curseur en fin de liste, on ne bouge pas le curseur
	if(list->cursor->next == NULL) return NULL;
	
	//Cas général, on avance le curseur et on le retourne
	list->cursor = list->cursor->next;
	
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
	
	return list->cursor;
}

int listCountElem(List* list){
	int size = 0;
	if(list == NULL) return size;
	
	Cell* savedCursor = list->cursor;
	goToHeadList(list);
	while(nextCell(list) != NULL) size++;
	list->cursor = savedCursor;
	
	return size;
}

bool isListEmpty(List* list){
	if(list == NULL) return true;
	
	if(list->size <= 0) return true;
	return false;
}

void insertHeadCell(List* list, Cell* c){
	//La liste doit être initialisée (sinon ça devient le bordel)
	if(list == NULL || c == NULL){
		fprintf(stderr,"Tentative d'insertion d'un élement dans une liste nulle.\n");	
		return;
	} 
	
	c->previous = NULL;
	c->next = list->head;
	list->head->previous = c;
	list->head = c;
	
	list->size++;
}

void insertBottomCell(List* list, Cell* c){
	//La liste doit être initialisée (sinon ça devient le bordel)
	if(list == NULL || c == NULL){
		fprintf(stderr,"Tentative d'insertion d'un élement dans une liste nulle.\n");	
		return;
	} 
	
	c->previous = list->bottom;
	list->bottom->next = c;
	c->next = NULL;
	list->bottom = c;
	
	list->size++;
}

void insertAfterCell(List* list, Cell* cToInsert){
	if(list == NULL) return;
	
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
}

void insertBeforeCell(List* list, Cell* cToInsert){
	if(list == NULL) return;
	
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
}

void goToHeadList(List* list){
	if(list == NULL) return;
	
	list->cursor = NULL;	
}

Cell* goToBottomCell(List* list){
	if(list == NULL) return NULL;
	
	list->cursor = list->bottom;
	
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

Cell* delCellByPosition(List* list, int position){
	if(list == NULL || position < 0 || position > list->size)
		return NULL;
		
	Cell* theCell = getCellByPosition(list, position);
	
	//size-- est fait dans delCellInList
	return delCellInList(list, theCell);
}

void freeCellByPosition(List* list, int position){
	//delCellByPosition fait les tests pour nous...
	
	Cell* theCell = delCellByPosition(list,position);
	
	freeCell(theCell);
}

void freeCell(Cell* c){
	free(c);
}

void* delCellInList(List* list, Cell* theCell){
	if(list == NULL || theCell == NULL) return NULL;
	
	//On coupe les liens !
	
	//Cas spécial : le Cell est en fin de liste
	if(theCell == list->bottom){
		theCell->previous->next = NULL;
		list->bottom = theCell->previous;
		theCell->previous = NULL;
		
		//Si le curseur était sur theCell..
		if(list->cursor == theCell){
			list->cursor = list->bottom;
		}

	}
	//Cas spécial : le Cell est en tête de liste
	else if(theCell == list->head){
		theCell->next->previous = NULL;
		list->head = theCell->next;
		theCell->next = NULL;
		
		//Si le curseur était sur theCell..
		if(list->cursor == theCell){
			list->cursor = list->head;
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
		
		theCell->next = NULL;
		theCell->previous = NULL;
	}	
	
	list->size--;
	return theCell;
}

void freeCellInList(List* list, Cell* c){
	Cell* theCell = delCellInList(list,c);
	
	freeCell(theCell);
}

//Surcouche de free au cas où l'implémentation de List changerait
void freeList(List* list){
	free(list);
}

void freeListComplete(List* list){
	if(list == NULL) return;
	
	while(nextCell(list) != NULL){
		freeCell(list->cursor);
	}
	
	freeList(list);
}

void dumpList(List* list){
	if(list == NULL) return;
	int cnt = 1;
	
	printf("\n--------------Affichage Liste--------------\n\n");
	if(isListEmpty(list)){
		printf("\tListe vide.\n");
		return;
	}
	
	Cell* savedCursor = list->cursor;
	goToHeadList(list);

	
	while(nextCell(list) != NULL){
		printf("\n\tCellule %d) ", cnt);
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
	printf("\n----->Fin Affichage Liste\n");
	
	list->cursor = savedCursor;
}

