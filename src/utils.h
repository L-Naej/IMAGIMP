#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

/**
 * Permet de simuler le type booléen.
 */
typedef enum {false=0, true=1} bool;  

/**
 * Surcharge fread pour s'assurer que tous
 * les unsigned char voulus sont lus. 
 * Renvoie true si tout s'est bien passé.
 * @param t le tableau dans lequel sont stockés les unsigned char lus.
 * @param n le nombre d'élément à lire
 * @param f le fichier dans lequel lire
 */
bool readNUchar(unsigned char* t, int n, FILE* f);

/**
 * Surcharge fwrite pour s'assurer que tous
 * les unsigned char voulus sont écrits. 
 * Renvoie true si tout s'est bien passé.
 * @param t le tableau dans lequel sont lus les unsigned char écrits.
 * @param n le nombre d'élément à lire
 * @param f le fichier dans lequel lire
 */
bool writeNUchar(const unsigned char*, int n, FILE* f);

/**
 * Inverse une image PPM représentée par un tableau
 * d'unsigned char.
 */
void invertPPMArray(unsigned char array[], long int width, long int height);

/**
 * Affiche le contenu du dossier dirName dans la console.
 * Retourne la longueur du plus long nom de fichier.
 */
int printDirectory(const char dirName[]);

/**
 * Vide le buffer d'entrée stdin.
 */
void clearStdin();

#endif
