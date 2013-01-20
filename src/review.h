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
	int ownerId;
	char* imgName;
}SavedLut;

typedef struct saved_layer{
	Layer* ptrLay;
	double oldOpacity;
	double newOpacity;
	LAYER_OP oldOperation;
	LAYER_OP newOperation;
	int position;
	int id;
	char* imgName;
	int width;
	int height;
	bool atEnd;//Indique si le calque est en fin de liste au 
		     //moment de la sauvegarde dans l'historique
	SavedLut* lstLuts;
	int nbLuts;
} SavedLayer;

typedef union core_review{
	SavedLayer* savedLayer;
	SavedLut* savedLut;
	char* imgName;
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

/**
 * Initialise l'historique. Si un
 * historique existait déjà en mémoire, il est
 * supprimé.
 */
void initReview();

/**
 * Ajoute une opération dans la pile
 * de l'historique.
 */
void pushOperation(Operation* op);

/**
 * Retire la dernière opération ajoutée
 * à la pile.
 */
Operation* popOperation();

/**
 * Annule la dernière opération ajoutée à l'historique.
 */
void undo();

/**
 * Enregistre une opération liée à un calque.
 */
bool recordLayerOperation(List* layerList, Layer* lay, OperationName opName, bool atEnd);

/**
 * Enregistre une opération liée au chargement de l'image
 * initiale.
 */
bool recordImgOperation(Image* img, OperationName opName);

/**
 * Enregistre une opération liée à un LUT.
 */
bool recordLutOperation(List* lutList, Layer* owner, Lut* lt, OperationName opName);

/**
 * Affiche l'historique dans la console.
 */
void displayReview();

/**
 * Affiche une opération dans la console.
 */
void printOperation(Operation* curOp);

/**
 * Créé la structure Operation. C'est cette structure
 * qui est stockée dans la pile historique.
 */
Operation* createOperation(OperationName name, ReviewType type, List* list);

/**
 * Libère la mémoire prise par une opération.
 */
void freeOperation(Operation* op);

/**
 * Libère la mémoire de l'historique.
 */
void freeReview();
#endif
