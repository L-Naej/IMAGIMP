#ifndef REVIEW_H
#define REVIEW_H
#include "layer.h"
#include "image.h"
#include "lut.h"
#include "list.h"

typedef enum operation_name{
	IM1,
	CAL1,
	CAL3,
	CAL4,
	CAL5,
	LUT1,
	LUT3
} OperationName;

typedef struct saved_layer{
	Layer* ptrLay;
	double oldOpacity;
	double newOpacity;
	LAYER_OP oldOperation;
	LAYER_OP newOperation;
	int position;
} SavedLayer;

/**
 * Informations sur le lut utilisées
 * pour le régénérer si on veut annuler
 * sa suppression. (sauvegarder tout le lut serait trop lourd)
 * 
 * Note : on supprime forcément le lut courant, donc
 * pour le régénérer, son input est l'output du lut actuel
 * inutile de le sauvegarder.
 * 
 */
typedef struct saved_lut{
	Layer* owner;
	LUT_FUNCTION function;
	int functionValue;
	int maxValue;
}SavedLut;

typedef union core_review{
	SavedLayer* savedLayer;
	SavedLut* savedLut;
	Image* img;
}ReviewType;

/* /!\Pourquoi sauvegarder la lutList ? => si l'utilisateur supprime un lut,
 * change de calque, puis annule => le changement de calque n'est pas répertorié dans
 * l'historique ainsi le lut se rajouterait au calque courant, ERREUR !
 * On garde donc une référence vers la liste de lut auquel ce dernier appartient
 */
typedef struct operation{
	OperationName name;
	ReviewType type;
	List* list;//Nécessaire pour les annulations LUT et Layer
}Operation;

void initReview();
void pushOperation(Operation* op);
Operation* popOperation();

void undo();

bool recordLayerOperation(List* layerList, Layer* lay, OperationName opName);
bool recordImgOperation(Image* img, OperationName opName);
bool recordLutOperation(List* lutList, Layer* owner, Lut* lt, OperationName opName);

void displayReview();
void printOperation(Operation* curOp);

Operation* createOperation(OperationName name, ReviewType type, List* list);

void freeOperation(Operation* op);
void freeReview();
#endif
