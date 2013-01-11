#ifndef LAYER_H
#define LAYER_H
#include "image.h"
#include "lut.h"
#include "list.h"

/**
 * Un layer possède une image source qui n'est jamais modifiée
 * et une image finale qui est le résultat de l'application des
 * effets (LUT) à l'image source. 
 *
 * NOTES : 
 * - Layer = Calque
 * - Chaque layer possède au moins un LUT "neutre" qui se
 * contente de laisser l'image telle qu'elle.
 * Cela permet de chainer les luts lors de la génération de
 * l'image finale sans se soucier de l'entrée à donner
 * au premier LUT de la liste. Ainsi chaque lut prend en entrée
 * la sortie du lut précédent, et la chaîne démarre par le LUT
 * neutre.
 * L'image finale est une nouvelle image de même dimensions que 
 * l'image source, et représentant le résultat de l'application
 * des effets sur l'image source.
 * 
 * Pour le fonctionnement en détail des LUT et de leur application au layer,
 * voir lut.h
 */


typedef enum layer_operation{
	SUM = 1,
	MULTIPLICATION = 2
} LAYER_OP;


typedef struct layer{
	int id;
	Image* imgSource;
	Image* imgFinale;
	Image* histoSource;
	Image* histoFinal;
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
 * Créé le lut associé à la fonction "function" avec pour
 * paramètre val et l'ajoute au layer lay. Le lut lt est ajouté
 * en fin de la liste des lut courants du layer lay.
 * Retourne vrai si l'ajout a réussi, faux sinon. 
 */
bool addLut(Layer* lay, LUT_FUNCTION function, int functionValue);

/**
 * Supprime de la liste le dernier lut ajouté sauf s'il
 * s'agit du lut neutral. Renvoie le lut supprimé de la liste.
 * (il n'est pas supprimé de la mémoire pour pouvoir être géré dans l'historique)
 */
Lut* delLastLut(Layer* lay);

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
 * tous les LUT du layer. C'est cette fonction qui gère sépia.
 * En effet normalement seule l'application du dernier LUT doit suffire
 * à générer l'image finale. Seulement sépia nécessite un passage de l'image en niveaux de gris.
 * Cette fonction se charge donc en fait de détecter si sépia est présent, puis de recalculer
 * tous les LUT qui suivent chaque fonction sépia car sépia "casse" la chaîne.
 */
void applyLuts(Layer* lay);

/*
 * Génère l'histogramme final de l'image du calque.
 * C'est à dire de l'image après application des effets.
 */
bool generateFinalHistogram(Layer* lay);

#endif
