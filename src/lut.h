#ifndef LUT_H
#define LUT_H
#include "image.h"

/**
 * ARCHITECTURE DES LUT :
 * Chaque LUT possède une structure Channels. Cette structure
 * représente une LUT par canal de couleur (r,g,b mais cela pourrait aussi bien
 * être h,s,v ou y,u,v par exemple). C'est à dire que, pour calculer une image RGB finale il faut faire :
 * image->pixelR[i] = lut->chan1[i];
 * image->pixelG[i] = lut->chan2[i];
 * image->pixelB[i] = lut->chan3[i];
 * (ceci est du pseudo-code qui ne marcherait pas dans cette application).
 * 
 * 
 * Pour générer une LUT, il est nécessaire de lui fournir en entrée un Channels "input".
 * Cela permet d'optimiser les traitement, en effet à l'ajout d'une LUT il est inutile de calculer toute
 * la chaîne des LUT à nouveau, la dernière LUT se base sur la LUT qui la précède.
 * 
 * Chaque calque (layer) possède toujours la LUT NEUTRAL qui ne fait rien
 * (c'est-à-dire qu'à un pixel rouge qui vaut 0, cette LUT fait correspondre 0 etc). 
 * =>Pourquoi ?
 * Cela permet d'avoir un premier "input" pour les LUT qui suivent et ainsi chaîner les LUT
 * sans se soucier du début de la chaîne.
 * 
 * ATTENTION CAS SPECIAL :
 * Le Sépia : le sépia implique de transformer l'image en niveaux de gris.
 * Cela implique donc de modifier l'image dans la chaîne d'application des LUT...
 * Ce qui casse un peu le principe d'une LUT, mais bon. 
 * Pour générer du sépia il faut donc IMPERATIVEMENT passer par la fonction
 * createSepiaLut(Image* img) qui demande donc l'image en entrée.
 * Ca casse un peu l'architecture, ça oblige de faire des traitements spéciaux sur
 * l'image finale du calque avant même d'appliquer la LUT. 
 * Surtout, cela implique aussi que lors de la suppression d'une LUT, si ce LUT était sépia, 
 * il faut restaurer les couleurs et donc recalculer toute la chaîne de traitement !
 * 
 * ALGORITHME DE GENERATION DE L'IMAGE FINALE SUR *UN* LAYER
 * 
 * -Ajout d'une lut
 * Si NON SEPIA
 * 	-calcul de cette lut à partir de la lut précédente
 * SI SEPIA
 * 	-transformation de l'image finale en niveaux de gris
 * 	-calcul de sepia à partir de l'image finale du LUT
 * 
 * Application de la lut sur l'image finale en lui donnant la dernière LUT.
 * 
 */
 
 
/**
 * Identifiants (flags) des fonctions
 * d'effet (LUT)
 */
typedef enum lut_function{
	INVERT = 1,
	ADDLUM = 2,
	DIMLUM = 3,
	ADDCON = 4,
	DIMCON = 5,
	SEPIA = 6,
	GRAY = 7,
	NEUTRAL = 0
	} LUT_FUNCTION;

/**
 * Représente trois LUT sur canaux de couleur.
 * RGB : chan1 = R; chan2 = G; chan3 = B
 */
typedef struct {
	unsigned char* chan1;
	unsigned char* chan2;
	unsigned char* chan3;
	
	int size;
} Channels;

/**
 * Structure qui regroupe les channels
 * et les paramètres de l'effet.
 */
typedef struct lut{
	Channels* channels;
	long int size;
	int maxValue;
	LUT_FUNCTION function;
	int valueEffect;
}Lut;



/**
 * Fonction de création de LUT.
 * Attention : pour le lut sépia appeler createSepiaLut(Image*)
 */
Lut* createLut(Channels* input, LUT_FUNCTION lF, int val, int maxVal);

/**
 * Régénère le lut *lt à partir de l'input input.
 * Image nécessaire si lut sépia
 */
void regenerateLut(Lut** lt, Channels* input, Image* img);

void freeLut(Lut* lt);

/**
 * Applique un LUT à l'image source et fourni le
 * résultat dans imgFinale.
 */
void applyLutToImg(Image* imgSource, Image* imgFinale, Lut* lut);

void dumpLut(Lut* lt);

Channels* allocChannels(int size);
Channels* createChanFromRGB(unsigned char* arrayRGB, int size);
void freeChannels(Channels* chan);
#endif
