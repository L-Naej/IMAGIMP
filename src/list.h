#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include "utils.h"

/**
 * Permet de connaître le type de liste
 * à l'exécution. Un type UNKNOWN est prévu
 * au cas où.
 */
typedef enum list_type{
	LAYER,
	LUT,
	UNKNOWN
} LIST_TYPE;

/**
 * Représente une cellule d'une liste,
 * donc reliée à deux autres cellules (ou
 * une seule si en tête ou queue de liste).
 */
typedef struct cell{
	struct cell* next;
	struct cell* previous;
	void* userData;
} Cell;

/**
 * Représente la liste en elle même. 
 * Un utilisateur de cette bibliothèque ne devrait
 * utiliser que cette structure.
 */
typedef struct list{
	Cell* head;
	Cell* cursor;/*Le curseur permet de naviguer dans la liste*/
	Cell* bottom;
	
	int size;
	LIST_TYPE type;
} List;

/**
 * Créé une cellule dont le champs Cell->userData pointe 
 * sur userData.
 * Cette cellule n'est reliée à aucune autre.
 */
Cell* createCell(void* userData);

/**
 * Créé une liste de Cell vide.
 */ 
List* createEmptyList(LIST_TYPE type);
/**
 * Créé une liste de Cell ayant pour premier élément
 * head. Place le curseur en tête de liste (à NULL, 
 * cf NOTE GENERALE).
 * @param head le premier élément de la liste créée.
 */
List* createList(LIST_TYPE type, void* headUserData);

/**
 * Renvoie le Cell courrant de la liste list
 * (le Cell sur lequel pointe le curseur).
 */
Cell* currentCell(List* list);

/**
 * Déplace le curseur de la liste sur le
 * prochain Cell et le renvoie.
 */
Cell* nextCell(List* list);

/**
 * Déplace le curseur sur le Cell précédent de la
 * liste et le renvoie.
 */
Cell* previousCell(List* list);

/**
 * Compte et renvoie le nombre d'éléments que contient la liste.
 * Ne devrait pas être appelée par une fonctione extérieure de la librairie,
 * utiliser le champ nbCells de la structure List à la place.
 * Renvoie 0 si list est égal à NULL
 * @param list la liste que l'on compte
 * Note: voir l'utilité de cette fonction avec le champs nbCells...
 */
int listCountElem(List* list);

/**
 * Test si une liste est vide.
 * Renvoie true si list==NULL.
 */
bool isListEmpty(List* list);

/**
 * Insère un Cell en tête de liste.
 * Attention : écrase les liens que pouvait avoir ce
 * Cell avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertHeadCell(List* list, void* userData);

/**
 * Insère un Cell en fin de liste.
 * Attention : écrase les liens que pouvait avoir ce
 * Cell avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertBottomCell(List* list, void* userData);

/**
 * Insère le Cell lToInsert à la suite du curseur.
 * Attention : écrase les liens que pouvait avoir ce
 * Cell avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertAfterCell(List* list, void* userData);

/**
 * Insère le Cell lToInsert avant le curseur.
 * Attention : écrase les liens que pouvait avoir ce
 * Cell avec d'autres.
 * Ne déplace pas le curseur.
 */
void insertBeforeCell(List* list, void* userData);

/**
 * Positionne le curseur en tête de liste (donc à NULL).
 * Attention! : un insertAfterCell() après un appel à cette fonction
 * positionnera le nouveau Cell en tête de liste (et non en seconde
 * position). insertBeforeCell() fera de même.
 */
void goToHeadList(List* list);

/**
 * Positionne le curseur en fin de liste.
 * Renvoie le pointeur vers le dernier élément de la liste.
 */
Cell* goToBottomCell(List* list);

/**
 * Renvoie le Cell de la liste étant à la position
 * demandée (le premier Cell est à la position 1).
 */
Cell* getCellByPosition(List* list, int position);

/**
 * Enlève le Cell de la liste étant à la position
 * demandée (le premier Cell est à la position 1).
 * Ne libère pas la mémoire allouée, le Cell existe toujours.
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Renvoie le pointeur vers le Cell enlevé de la liste.
 */
Cell* delCellByPosition(List* list, int position);

/**
 * Supprime le Cell de la liste étant à la position
 * demandée (le premier Cell est à la position 1).
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Libère la mémoire allouée, userData n'existe plus.
 */
void freeCellByPosition(List* list, int position);

/**
 * Vide la mémoire prise par la Cell pointée par
 * cell.
 */
void freeCell(Cell* cell);

/**
 * Enlève le Cell l de la liste.
 * Ne libère pas la mémoire allouée, le userData du Cell existe toujours.
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Renvoie le pointeur vers le userData du Cell enlevé de la liste.
 */
void* delCellInList(List* list, Cell* c);

/**
 * Supprime le Cell l de la liste.
 * Si le curseur était sur le Cell enlevé, il va sur le Cell 
 * ayant pris sa position dans la liste.
 * Libère la mémoire allouée, le Cell n'existe plus.
 */
void freeCellInList(List* list, Cell* c);

/*
 * Libère une liste de la zone mémoire
 * mais ne supprime pas ses cellules (les Cells
 * qu'elle contient).
 */
void freeList(List* list);

/*
 * Libère une liste de la zone mémoire
 * ainsi que les cellules (Cells) qu'elle contient.
 */
void freeListComplete(List* list);

/**
 * Affiche une liste dans la console (debug, stderr)
 */
void dumpList(List* list);
#endif

