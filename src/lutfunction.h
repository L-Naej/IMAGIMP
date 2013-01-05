#ifndef LUT_FUNC_H
#define LUT_FUNC_H
#include "lut.h"

/**
 * Ensemble des fonctions qui "remplissent"
 * la structure LUT avec les valeurs voulues
 * en fonction de l'effet.
 * 
 * Les LUT pouvant être chaînées ces fonctions ont
 * besoin d'un input.
 * 
 * Sépia est un cas à part qui "casse" la chaîne des lut,
 * car son input n'est pas un LUT, il nécessite en fait que l'image
 * soit en niveaux de gris. Ainsi le pipeline des LUT
 * au niveau de Sépia est géré au niveau de la liste des lut
 * dans layer.c, cf. fonction applyLuts(Layer*).
 * Tous les LUT qui suivent sépia sont "régénérés"
 * après que l'image ait été convertie en niveaux de gris 
 * et sépia calculé.
 */
 
void neutral(Lut* lt, Channels* input);

void invertLut(Lut* lt, Channels* input);

void addLum(Lut* lt, Channels* input);

void dimLum(Lut* lt, Channels* input);

void changeContrast(Lut* lt, Channels* input);

void sepia(Lut* lt);

/**
 * imgDest devient imgSource en niveaux de gris.
 * imgDest doit être un pointeur vers une zone mémoire
 * allouée.
 */
bool imgToGray(Image* imgSource, Image* imgDest);

#endif
