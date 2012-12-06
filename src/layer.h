#ifndef LAYER_H
#define LAYER_H
#include "image.h"
#include "lut.h"
#include "list.h"

/** NOTE : Layer = Calque **/

typedef enum layer_operation{
	SUM,
	MULTIPLICATION
} LAYER_OP;


typedef struct layer{
	int id;
	Image* imgSource;
	double opacity;
	LAYER_OP operation;
	List* lutList;
	
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
 * Créé un layer vierge dont l'image source à une taille de w*h.
 * L’ajout d’un calque vierge est l’insertion d’un calque multiplicatif, d’opacité ́nulle et dont
 * l’image est une image remplie de pixel blanc pur. Cette insertion s’effectue à la fin de la liste                                                                          a
 * des calques.
 */
Layer* createEmptyLayer(int w, int h);

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

/**
 * Ajoute le lut lt au layer lay. Le lut lt est ajouté
 * en fin de la liste des lut courants du layer lay.
 * Retourne vrai si l'ajout a réussi, faux sinon. 
 */
bool addLut(Layer* lay, Lut* lt);

/**
 * Modifie l'opacité du layer lay en lui affectant la
 * valeur newOpa.
 */
void setLayerOpacity(Layer* lay, double newOpa);

/**
 * Modifie l'opération (somme ou mutliplication) utilisée
 * par le layer pour se mixer au layer dessous lui.
 */
void setLayerOperation(Layer* lay, LAYER_OP newOp);

#endif
