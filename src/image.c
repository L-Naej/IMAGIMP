#include "image.h"
#include "utils.h"
#include "../lib/include/interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 70
#define N_HEADER_DATALINE 3

struct image{
	short format;//P1 to P6
	char* comments;
	int width;
	int height;
	int maxValue;
	unsigned char* tabRVB;
	
};

Image* loadImage(char* fileName){
	if(fileName == NULL){
		fprintf(stderr, "Le nom du fichier image est vide.\n");
		return NULL;
	}
	
	FILE* imgFile = NULL;
	Image* img = NULL;
	char* comments = NULL;
	
	char currentTxt[MAX_LINE_LENGTH];
	unsigned char currentPix;
	
	long position = 0;
	short ctr = 0;
	long int i, j,nPix;
	
	imgFile = fopen(fileName, "r");
	if(imgFile==NULL){
		fprintf(stderr, "Impossible d'ouvrir le fichier image.\n");
		return NULL;
	}
	
	img = (Image*) malloc(sizeof(Image));
	if(img == NULL){
		fclose(imgFile);
		return NULL;
	}
	
	
	while(ctr < N_HEADER_DATALINE && fgets(currentTxt, MAX_LINE_LENGTH, imgFile) != NULL){
		if(currentTxt[0] == '#'){
			if(img->comments == NULL){
				img->comments = (char*) malloc(strlen(currentTxt)*sizeof(char));
			}
			else{
				img->comments = (char*) realloc(img,
					(strlen(img->comments)+strlen(currentTxt))*sizeof(char));
			}
			continue;	
		}
		
		switch(ctr){
			case 0 : img->format = (short) atoi((currentTxt+1));
			break;
			case 1 : detectWH(currentTxt, &img->width, &img->height);
			break;
			case 2 : img->maxValue = atoi(currentTxt);
			break;
		}
		ctr++;
	}
	
	position = ftell(imgFile);
	
	fclose(imgFile);
	
	
	//Allocation mémoire pour les lignes de pixels
	nPix = img->width*img->height;
	img->tabRVB = (unsigned char*) malloc(nPix*3*sizeof(unsigned char));//1px = 3 composantes
	if(img->tabRVB == NULL){
		free(img->comments);
		free(img);
	}
	
	
	//Lecture des pixels en binaire
	imgFile = fopen(fileName, "rb");
	fseek(imgFile,position, SEEK_SET);
	
	ctr = 0;
	
	/*On commence par le dernier pixel (image stockées 'à l'envers' avec le 
	haut de l'image en bas du tableau). On décale de trois pour prendre les composantes dans le bon ordre*/
	for(i = nPix*3-3; i >= 0; i=i-3){		
		readNUchar(&currentPix, 1, imgFile);
		img->tabRVB[i] = currentPix;
		readNUchar(&currentPix, 1, imgFile);
		img->tabRVB[i+1] = currentPix;
		readNUchar(&currentPix, 1, imgFile);
		img->tabRVB[i+2] = currentPix;
	}

	fclose(imgFile);
	
	return img;
}

/**
 * Détecte la largeur et la hauteur de l'image contenues dans une
 * chaîne de caractère de la forme "largeur hauteur \n"
 */
void detectWH(const char* text, int* w, int* h){
	if(text==NULL)return;
	char sW[4]; char sH[4];
	short i = 0, j = 0;
	
	int sText = strlen(text);
	
	while(!isspace(text[i]) && i < 4){
		sW[i] = text[i];
		i++;
	}
	
	*w = atoi(sW);
	while(text[i+j] != '\n' && j < 4 && (i+j) != sText){
		sH[j] = text[i+j];
		j++;
	}
	
	*h = atoi(sH);
}

void freeImage(Image* img){
	free(img->comments);
	free(img->tabRVB);
	free(img);
}


//Fonction de test à supprimer
void main(void){
	Image* test = loadImage("../images/Lena.512.ppm");
	if(test)
		initGLIMAGIMP(test->width,test->height,test->tabRVB);
	free(test);
	
}
