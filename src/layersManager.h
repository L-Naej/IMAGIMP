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
 * Déplace le curseur en fin de liste (donc sur le dernier layer ajouté).
 * Renvoie true si l'ajout réussi, false sinon.
 */
bool addLayer(List* layerList, Layer* newLay);

/**
 * Génère l'image finale en partant du premier calque
 * et en allant jusqu'au dernier en appliquant toutes
 * les LUT de chaque calque. Stocke le résultat en 
 * peuplant le champ arrayRGB de "finalImage". 
 * finalImage ne doit pas valoir NULL.
 * Le champ maxValue de finalImage doit être juste (utilisé
 * par la fonction)
 * 
 * Renvoie true si la génération s'est bien passée, false sinon.
 * (à voir si c'est vraiment utile)
 */
bool generateFinalImage(List* layerList, Image* finalImage);

/**
 * Fonction récursive appelée par generateFinalImage.
 * A ne pas appeler depuis l'exterieur de layersManager.c
 * Toujours utiliser generateFinalImage.
 */
void genFinalImageRecur(List* list, Layer* lay, Image* finalImage);

#endif
