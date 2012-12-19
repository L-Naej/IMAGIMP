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

typedef union core_review{
	SavedLayer* savedLayer;
	Lut* lut;
	Image* img;
}ReviewType;

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

void displayReview();

Operation* createOperation(OperationName name, ReviewType type, List* list);

void freeOperation(Operation* op);
void freeReview();
#endif
