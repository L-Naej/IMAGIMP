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
	
	struct layer* nextLayer;
	struct layer* previousLayer;
	
} Layer;


typedef struct layerList{
	Layer* head;
	Layer* cursor;/*Le curseur permet de naviguer dans la liste*/
	Layer* bottom;
	
	int nbLayers;
} LayerList;

/**
 * Créé un layer avec
 * @param source l'image source du layer
 * @param previousLayer le layer en dessous de lui, passer NULL
 * pour indiquer que ce layer n'a pas de layer en dessous de lui.
 * @param opa le paramètre d'opacité initial du calque
 * @param operation l'operation que va utiliser le calque pour
 * se lier au calque en dessous de lui (somme ou multplication)
 */
Layer* createLayer(Image* source, Layer* previousLayer, double opa, LAYER_OP operation);

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
 * NOTE GENERALE SUR LES LISTES :
 * Le curseur démarre en dehors de la liste (cursor==NULL) et s'arrête sur le
 * dernier élément de la liste. C'est à dire que :
 * Si le curseur a été remis en première position,
 * nextLayer() renverra head  et   
 * currentLayer() renverra NULL.
 * Par contre si le curseur est en fin de liste, 
 * previousLayer() renverra bien l'avant dernier layer de la liste et
 * currentLayer() renverra le dernier layer de la liste.
 */

/**
 * Créé une liste de layer vide.
 */ 
LayerList* createEmptyLayerList();
/**
 * Créé une liste de layer ayant pour premier élément
 * head. Place le curseur en tête de liste (à NULL, 
 * cf NOTE GENERALE).
 * @param head le premier élément de la liste créée.
 * Attention : l'éventuel lien vers un précédent layer
 * lié à head est coupé, seul son layer suivant est
 * conservé.
 */
LayerList* createLayerList(Layer* head);

/**
 * Renvoie le layer courrant de la liste list
 * (le layer sur lequel pointe le curseur).
 */
Layer* currentLayer(LayerList* list);

/**
 * Déplace le curseur de la liste sur le
 * prochain layer et le renvoie.
 */
Layer* nextLayer(LayerList* list);

/**
 * Déplace le curseur sur le layer précédent de la
 * liste et le renvoie.
 */
Layer* previousLayer(LayerList* list);

/**
 * Compte et renvoie le nombre d'éléments que contient la liste.
 * Ne devrait pas être appelée par une fonctione extérieure de la librairie,
 * utiliser le champ nbLayers de la structure LayerList à la place.
 * Renvoie 0 si list est égal à NULL
 * @param list la liste que l'on compte
 * Note: voir l'utilité de cette fonction avec le champs nbLayers...
 */
int layerListCountElem(LayerList* list);

/**
 * Test si une liste est vide.
 * Renvoie true si list==NULL.
 */
bool isLayerListEmpty(LayerList* list);

/**
 * Insère un layer en tête de liste.
 * Attention : écrase les liens que pouvait avoir ce
 * layer avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertHeadLayer(LayerList* list, Layer* l);

/**
 * Insère un layer en fin de liste.
 * Attention : écrase les liens que pouvait avoir ce
 * layer avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertBottomLayer(LayerList* list, Layer* l);

/**
 * Insère le layer lToInsert à la suite du curseur.
 * Attention : écrase les liens que pouvait avoir ce
 * layer avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertAfterLayer(LayerList* list, Layer* lToInsert);

/**
 * Insère le layer lToInsert avant le curseur.
 * Attention : écrase les liens que pouvait avoir ce
 * layer avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertBeforeLayer(LayerList* list, Layer* lToInsert);

/**
 * Positionne le curseur en tête de liste (donc à NULL).
 */
void goToHeadLayer(LayerList* list);

/**
 * Positionne le curseur en fin de liste.
 * Renvoie le pointeur vers le dernier élément de la liste.
 */
Layer* goToBottomLayer(LayerList* list);

/**
 * Renvoie le layer de la liste étant à la position
 * demandée (le premier layer est à la position 1).
 */
Layer* getLayerByPosition(LayerList* list, int position);

/**
 * Enlève le layer de la liste étant à la position
 * demandée (le premier layer est à la position 1).
 * Ne libère pas la mémoire allouée, le layer existe toujours.
 * Si le curseur était sur le layer enlevé, il va sur le layer 
 * ayant pris sa position dans la liste.
 * Renvoie le pointeur vers le layer enlevé de la liste.
 */
Layer* delLayerByPosition(LayerList* list, int position);

/**
 * Supprime le layer de la liste étant à la position
 * demandée (le premier layer est à la position 1).
 * Si le curseur était sur le layer enlevé, il va sur le layer 
 * ayant pris sa position dans la liste.
 * Libère la mémoire allouée, le layer n'existe plus.
 */
void freeLayerByPosition(LayerList* list, int position);

/**
 * Enlève le layer l de la liste.
 * Ne libère pas la mémoire allouée, le layer existe toujours.
 * Si le curseur était sur le layer enlevé, il va sur le layer 
 * ayant pris sa position dans la liste.
 * Renvoie le pointeur vers le layer enlevé de la liste.
 */
Layer* delLayerInList(LayerList* list, Layer* l);

/**
 * Supprime le layer l de la liste.
 * Si le curseur était sur le layer enlevé, il va sur le layer 
 * ayant pris sa position dans la liste.
 * Libère la mémoire allouée, le layer n'existe plus.
 */
void freeLayerInList(LayerList* list, Layer* l);

/*
 * Libère une liste de la zone mémoire
 * mais ne supprime pas ses cellules (les layers
 * qu'elle contient).
 */
void freeLayerList(LayerList* list);

/*
 * Libère une liste de la zone mémoire
 * ainsi que les cellules (layers) qu'elle contient.
 */
void freeLayerListComplete(LayerList* list);

/**
 * Affiche une liste dans la console (debug, stderr)
 */
void dumpLayerList(LayerList* list);


#endif
