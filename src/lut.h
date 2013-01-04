#ifndef LUT_H
#define LUT_H
#include "image.h"

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
 * Création du lut sépia demande des manipulations spéciales.
 * Il faut passer par cette fonction et lui donner l'image sur
 * laquelle travailer.
 */
Lut* createSepiaLut(Image* imgSource);

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
