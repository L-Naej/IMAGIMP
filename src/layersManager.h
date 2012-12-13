#ifndef LAYERS_MANAGER_H
#define LAYERS_MANAGER_H
#include "list.h"
#include "layer.h"
#include "image.h"

/**
 * Initialise la liste des layers utilisée par
 * l'application. Si l'initialisation échoue
 * pour une raison quelconque, quitte le programme
 * avec un message d'erreur.
 */
List* initLayersList(int argc, char** argv);

/**
 * Ajoute un layer à la fin de la liste de layer "layerList".
 * Déplace le curseur en fin de liste (donc sur le dernier layer ajouté)
 * si l'ajout à réussi, ne déplace pas le curseur sinon.
 * L'ajout n'est possible que si les dimensions du calque sont valides
 * et si ce dernier possède une image source.
 * Renvoie true si l'ajout réussi, false sinon.
 */
bool addLayer(List* layerList, Layer* newLay);

/**
 * "Surcharge" la fonction de liste pour
 * renvoyer un pointeur de Layer (et non un void*).
 * Vérifie que la liste est bien une liste de Layers
 * (renvoie NULL si ce n'est pas le cas).
 */
Layer* currentLayer(List* layerList);

/**
 * "Surcharge" la fonction de liste pour
 * renvoyer un pointeur de Layer (et non un void*).
 * Vérifie que la liste est bien une liste de Layers
 * (renvoie NULL si ce n'est pas le cas).
 */
Layer* nextLayer(List* layerList);

/**
 * "Surcharge" la fonction de liste pour
 * renvoyer un pointeur de Layer (et non un void*).
 * Vérifie que la liste est bien une liste de Layers
 * (renvoie NULL si ce n'est pas le cas).
 */
Layer* previousLayer(List* layerList);

/**
 * Génère l'image finale en partant du premier calque
 * et en allant jusqu'au dernier en appliquant toutes
 * les LUT de chaque calque. 
 * Alloue la mémoire pour stocker le résutalt dans *finalImage.
 *  
 * Le champ maxValue de finalImage doit être juste (utilisé
 * par la fonction)
 * 
 * Renvoie true si la génération s'est bien passée, false sinon.
 * (à voir si c'est vraiment utile)
 */
bool generateFinalImage(List* layerList, Image** finalImage);

/**
 * Fonction récursive appelée par generateFinalImage.
 * A ne pas appeler depuis l'exterieur de layersManager.c
 * Toujours utiliser generateFinalImage.
 */
void genFinalImageRecur(List* list, Layer* lay, Image* finalImage);

#endif
