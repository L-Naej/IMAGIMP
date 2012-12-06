#ifndef IMAGE_H
#define IMAGE_H
#include "utils.h"
typedef struct image{
	short format;//P1 to P6
	char* comments;
	char* name;
	int width;
	int height;
	int maxValue;
	unsigned char* arrayRVB;
	
}Image;

//Charge une image depuis le disque dur
Image* loadImage(char* fileName);

//Sauvegarde une image sur le disque dur
bool saveImage(Image* img);

//Créé une image vide blanche (tous les pixels à DEFAULT_MAX_VAL) de dimension w*h
Image* createEmptyImg(int w, int h);

/**
 * Détecte la largeur et la hauteur de l'image contenues dans une
 * chaîne de caractère de la forme "largeur hauteur \n"
 */
void detectWH(const char* text, int* w, int* h);

//Libère une image en mémoire pointée par img
void freeImage(Image* img);

//Ajoute ou modifie le nom d'une struct Image
bool imgAddName(Image* img, const char name[]);

#endif
