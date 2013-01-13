#ifndef IMAGE_H
#define IMAGE_H
#include "utils.h"

#define DEFAULT_MAX_VAL 255
#define NB_COL_COMP 3 //nombre de composantes couleur (RGB=>3)

/*
 * Représente une image stockée en mémoire.
 * En principe peut stocker n'importe quel type d'image
 * sous la forme d'un tableau à une dimension d'unsigned char
 * (donc valeur max d'une composante = 255).
 * Concrètement dans le programme ne stocke que des images
 * PPM P6. Les valeurs sont stockées dans un tableau qui accole 
 * les composantes RGB. 
 * L'image PPM est normalement inversée (si chargée par loadImage()) 
 * pour être affichée à l'endroit dans OpenGL. La fonction de sauvegarde
 * (saveImage()) se charge de la remettre dans le sens valide PPM.
 */
typedef struct image{
	short format;//P1 to P6
	char* comments;
	char* name;
	int width;
	int height;
	int maxValue;
	unsigned char* arrayRGB;
	unsigned char* histoGrey;
	unsigned char* histoRGB;
	
}Image;

/*
 * Charge une image depuis le disque dur.
 * Attention : ne gère que le format PPM P6 (ASCII/binaire).
 * Si le fichier n'est pas de ce format, affiche
 * une erreur et renvoie NULL.
 */
Image* loadImage(char* fileName);

/* Sauvegarde une image sur le disque dur (peu importe son format
 * normalement mais notre programme ne gère que le PPM P6).
 * Renvoie true si la sauvegarde s'est bien passée,
 * false sinon.
 */
bool saveImage(Image* img);

/*
 * Créé une image vide blanche (tous les pixels à maxValue) de dimension w*h
 * au format PPM P6 (ASCII/binaire).
 */
Image* createEmptyImg(int w, int h, int maxValue);

/**
 * Détecte la largeur et la hauteur de l'image contenues dans une
 * chaîne de caractère de la forme "largeur hauteur \n"
 */
void detectWH(const char* text, int* w, int* h);

//Libère une image en mémoire pointée par img
void freeImage(Image* img);

//Ajoute ou modifie le nom d'une struct Image
bool imgAddName(Image* img, const char name[]);

/**
 * Copie l'image passée en paramètre.
 * Copie exclusivement :
 * -tableau de pixels
 * -dimensions
 * -valeur maximale
 * -format
 * -nom
 */
Image* copyImage(Image* source);

bool histoRGB (Image* img, int** hR,  int** hG,  int** hB);

/**
 * Génère un tableau représentant l'histogramme
 * de l'image passée en paramètre.
 * L'histogramme créé est normalisé.
 */
bool histo(Image* img, long int** h);

/**
 * Normalise un tableau représentant un histogramme.
 * La valeur max de l'histogramme = 100.
 * (fonction utilisée par histo)
 */
void normalizeHisto(long int* histo, int size);

/**
 * Créé une image qui représente
 * l'histogramme de l'image passée en paramètre.
 * Utilise la fonction histo.
 */
Image* createHistogram(Image* source);

/*
 * Fonction de debug des paramètres
 * de la structure Image (affiche la valeur
 * des paramètres dans la console).
 */
void dumpImage(Image* img);

#endif
