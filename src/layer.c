#include "layer.h"
#include <stdlib.h>

/**
 * Variable globale pour générer l'id des layers.
 */
int cntLayerId = 0;

Layer* createLayer(Image* source, Layer* previousLayer, double opa, LAYER_OP operation){
	if(source == NULL) return NULL;
	
	Layer* l = (Layer*) malloc(sizeof(Layer));
	if(l == NULL) return NULL;
	
	l->id = ++cntLayerId;
	l->imgSource = source;
	l->opacity = opa;
	l->operation = operation;
	l->previousLayer = previousLayer;
	l->nextLayer = NULL;
	
	//Ce layer (l) est le successeur d'un autre layer (celui qui le précède)
	if(l->previousLayer != NULL){
		l->previousLayer->nextLayer = l;
	}
	
	return l;
}

//Surcouche de free au cas où l'implémentation de Layer changerait
void freeLayer(Layer* l){
	free(l);
}

void dumpLayer(Layer* l){
	if(l == NULL) return;
	
	printf("\n-----------Affichage Layer ID n° %d-----------\n\n", l->id);
	printf("Address : @%p\n", l);
	printf("Image source : %s\nOpacity : %f\n", l->imgSource->name, l->opacity);
	printf("Opération : ");
	switch(l->operation){
		case SUM : printf("SUM\n");
		break;
		case MULTIPLICATION : printf("MULTIPLICATION\n");
	}
	if(l->nextLayer != NULL)
		printf("Next Layer : %p (ID %d)\n", l->nextLayer, l->nextLayer->id);
	else
		printf("Next Layer : None\n");
	if(l->previousLayer != NULL)
		printf("Previous Layer: %p (ID %d)\n", l->previousLayer,l->previousLayer->id);	
	else
		printf("Previous Layer : None\n");
}

LayerList* createEmptyLayerList(){
	LayerList* list = (LayerList*) malloc(sizeof(LayerList));
	if(list == NULL) return NULL;
	
	list->head = list->cursor = list->bottom = NULL;
	list->nbLayers = 0;
	
	return list;
}

LayerList* createLayerList(Layer* head){
	LayerList* list = createEmptyLayerList();
	if(list == NULL) return NULL;
	
	list->head = head;
	head->previousLayer = NULL;
	
	//On place le curseur en début de liste (avant head)
	list->cursor = NULL;	
	
	//Vérifie que le layer "head" n'est pas déjà connecté
	//à d'autres layers, si oui, ils font donc partie de la liste
	//(ne devrait pas arriver)
	list->nbLayers = layerListCountElem(list);
	
	//On va en fin de liste (si le layer passé
	//en paramètre est lié à d'autres)
	while(nextLayer(list) != NULL);
	list->bottom = list->cursor;
	
	list->cursor = NULL;
	
	return list;
}

Layer* currentLayer(LayerList* list){
	if(list == NULL) return NULL;
	return list->cursor;
}

//Note: si curseur en début de list cursor == NULL
//	  si curseur en fin de liste, cursor->nextLayer == NULL
Layer* nextLayer(LayerList* list){
	if(list == NULL) return NULL;
	
	//Cas spécial : curseur en début de liste, le curseur avance donc sur head
	if(list->cursor == NULL){
		list->cursor = list->head;
		return list->cursor;
	}
	
	//Cas spécial : curseur en fin de liste, on ne bouge pas le curseur
	if(list->cursor->nextLayer == NULL) return NULL;
	
	//Cas général, on avance le curseur et on le retourne
	list->cursor = list->cursor->nextLayer;
	
	return list->cursor;
}

//Note: si curseur en début de list cursor == NULL
//	  si curseur en fin de liste, cursor->nextLayer == NULL
Layer* previousLayer(LayerList* list){
	if(list == NULL) return NULL;
	
	//Cas spécial : curseur en début de liste
	if(list->cursor == NULL){
		return NULL;
	}
	
	list->cursor = list->cursor->previousLayer;
	
	return list->cursor;
}

int layerListCountElem(LayerList* list){
	int nbLayers = 0;
	if(list == NULL) return nbLayers;
	
	while(nextLayer(list) != NULL) nbLayers++;
	
	return nbLayers;
}

bool isLayerListEmpty(LayerList* list){
	if(list == NULL) return true;
	
	if(list->nbLayers <= 0) return true;
	return false;
}

void insertHeadLayer(LayerList* list, Layer* l){
	//La liste doit être initialisée (sinon ça devient le bordel)
	if(list == NULL || l == NULL){
		fprintf(stderr,"Tentative d'insertion d'un élement dans une liste nulle.\n");	
		return;
	} 
	
	l->previousLayer = NULL;
	l->nextLayer = list->head;
	list->head = l;
	
	list->nbLayers++;
}

void insertBottomLayer(LayerList* list, Layer* l){
	//La liste doit être initialisée (sinon ça devient le bordel)
	if(list == NULL || l == NULL){
		fprintf(stderr,"Tentative d'insertion d'un élement dans une liste nulle.\n");	
		return;
	} 
	
	l->previousLayer = list->bottom;
	l->nextLayer = NULL;
	list->bottom = l;
	
	list->nbLayers++;
}

void insertAfterLayer(LayerList* list, Layer* lToInsert){
	if(list == NULL) return;
	
	//On relie lToInsert à son voisin de droite
	lToInsert->nextLayer = list->cursor->nextLayer;
	lToInsert->nextLayer->previousLayer = lToInsert;
	
	//On relie lToInsert à son voisin de gauche
	lToInsert->previousLayer = list->cursor;
	list->cursor->nextLayer = lToInsert;
	
	//Cas spécial : on a inséré après le dernier élément
	if(list->cursor == list->bottom){
		list->bottom = lToInsert;
	}
	
	list->nbLayers++;
}

void insertBeforeLayer(LayerList* list, Layer* lToInsert){
	if(list == NULL) return;
	
	//On relie lToInsert à son voisin de gauche
	lToInsert->previousLayer = list->cursor->previousLayer;
	lToInsert->previousLayer->nextLayer = lToInsert;
	
	//On relie lToInsert à son voisin de droite
	lToInsert->nextLayer = list->cursor;
	list->cursor->previousLayer = lToInsert;
	
	//Cas spécial: on a inséré avant la tête
	if(list->cursor == list->head){
		list->head = lToInsert;
	}
	
	list->nbLayers++;
}

void goToHeadLayer(LayerList* list){
	if(list == NULL) return;
	
	list->cursor = NULL;	
}

Layer* goToBottomLayer(LayerList* list){
	if(list == NULL) return NULL;
	
	list->cursor = list->bottom;
	
	return list->cursor;
}

Layer* getLayerByPosition(LayerList* list, int position){
	if(list == NULL || position <= 0 || position > list->nbLayers)
		return NULL;
	int i;
	
	//Cas spéciaux
	if(position == 1) return list->head;
	if(position == list->nbLayers) return list->bottom;
	
	//Optimisation, woulala !
	//Si la position est plus proche de la fin on part de la fin,
	//et vice-versa.
	if(position > (list->nbLayers / 2 )){
		goToBottomLayer(list);
		for(i = list->nbLayers; i >position; --i){
			previousLayer(list);
		}
	}
	else{
		goToHeadLayer(list);
		for(i = 0; i < position; ++i){
			nextLayer(list);
		}
	}
	
	return list->cursor;
}

Layer* delLayerByPosition(LayerList* list, int position){
	if(list == NULL || position < 0 || position > list->nbLayers)
		return NULL;
		
	Layer* theLayer = getLayerByPosition(list, position);
	
	return delLayerInList(list, theLayer);
}

void freeLayerByPosition(LayerList* list, int position){
	//delLayerByPosition fait les tests pour nous...
	
	Layer* theLayer = delLayerByPosition(list,position);
	
	freeLayer(theLayer);
}

Layer* delLayerInList(LayerList* list, Layer* theLayer){
	if(list == NULL || theLayer == NULL) return NULL;
	
	//On coupe les liens !
	
	//Cas spécial : le layer est en fin de liste
	if(theLayer == list->bottom){
		theLayer->previousLayer->nextLayer = NULL;
		list->bottom = theLayer->previousLayer;
		theLayer->previousLayer = NULL;
		
		//Si le curseur était sur theLayer..
		if(list->cursor == theLayer){
			list->cursor = list->bottom;
		}

	}
	//Cas spécial : le layer est en tête de liste
	else if(theLayer == list->head){
		theLayer->nextLayer->previousLayer = NULL;
		list->head = theLayer->nextLayer;
		theLayer->nextLayer = NULL;
		
		//Si le curseur était sur theLayer..
		if(list->cursor == theLayer){
			list->cursor = list->head;
		}
	}
	//Sinon cas général
	else{
		theLayer->nextLayer->previousLayer = theLayer->previousLayer;
		theLayer->previousLayer->nextLayer = theLayer->nextLayer;
	
		//Si le curseur était sur theLayer..
		if(list->cursor == theLayer){
			list->cursor = theLayer->nextLayer;
		}
		
		theLayer->nextLayer = NULL;
		theLayer->previousLayer = NULL;
	}	
	
	return theLayer;
}

void freeLayerInList(LayerList* list, Layer* l){
	Layer* theLayer = delLayerInList(list,l);
	
	freeLayer(theLayer);
}

//Surcouche de free au cas où l'implémentation de LayerList changerait
void freeLayerList(LayerList* list){
	free(list);
}

void freeLayerListComplete(LayerList* list){
	if(list == NULL) return;
	
	while(nextLayer(list) != NULL){
		freeLayer(list->cursor);
	}
	
	freeLayerList(list);
}

void dumpLayerList(LayerList* list){
	if(list == NULL) return;
	
	printf("\n--------------Affichage Liste--------------\n\n");
	if(isLayerListEmpty(list)){
		printf("Liste vide.\n");
		return;
	}
	
	Layer* savedCursor = list->cursor;
	goToHeadLayer(list);

	
	while(nextLayer(list) != NULL){
		dumpLayer(list->cursor);
	}
	printf("\n----->Fin Affichage Liste\n");
	
	list->cursor = savedCursor;
}

