#ifndef LAYER_H
#define LAYER_H
#include "image.h"
#include "lut.h"

typedef enum layer_operation{
	SUM,
	MULTIPLICATION
} LAYER_OP;


typedef struct layer{
	int id;
	Image* imgSource;
	double opacity;
	LAYER_OP operation;
	
} Layer;

/**
 * Créé un layer avec
 * @param source l'image source du layer
 * @param opa le paramètre d'opacité initial du calque
 * @param operation l'operation que va utiliser le calque pour
 * se lier au calque en dessous de lui (somme ou multplication)
 */
Layer* createLayer(Image* source, double opa, LAYER_OP operation);

/**
 * Libère la mémoire du layer pointé par l
 * Attention : n'enlève pas ce layer d'une éventuelle liste
 * (fonction bas niveau) et ne supprime pas les références à ce layer qui
 * peuvent exister dans d'autre layer.
 * @param l le layer à libérer
 */
void freeLayer(Layer* l);

/**
 * Affiche un layer dans la console. (debug)
 */
void dumpLayer(Layer* l);

#endif
