#ifndef LAYER_H
#define LAYER_H
#include "image.h"
#include "lut.h"
#include "list.h"

/**
 * NOTES : 
 * - Layer = Calque
 * - Chaque layer possède au moins un LUT "neutre" qui se
 * contente de laisser l'image telle qu'elle.
 * Cela permet de chainer les luts lors de la génération de
 * l'image finale sans se soucier de l'entrée à donner
 * au premier LUT de la liste. Ainsi chaque lut prend en entrée
 * la sortie du lut précédent, et la chaîne démarre par le LUT
 * neutre.
 */


typedef enum layer_operation{
	SUM = 1,
	MULTIPLICATION = 2
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

/**
 * Génère l'image final après passage à travers
 * tous les LUT du layer.
 */
Image* applyLuts(Layer* lay);

#endif
