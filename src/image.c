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
	char* name;
	int width;
	int height;
	int maxValue;
	unsigned char* arrayRVB;
	
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
	
	img = (Image*) calloc(1,sizeof(Image));
	if(img == NULL){
		fclose(imgFile);
		return NULL;
	}
	
	
	img->name = (char*) malloc((strlen(fileName)+1)*sizeof(char));
	if(img->name == NULL){
		fprintf(stderr,"Impossible de récupérer le nom de l'image.\n");
	}
	else{
		strcpy(img->name,fileName);
	}
	
	while(ctr < N_HEADER_DATALINE && fgets(currentTxt, MAX_LINE_LENGTH, imgFile) != NULL){
		//Gestion des commentaires
		if(currentTxt[0] == '#'){
			if(img->comments == NULL){
				img->comments = (char*) malloc(strlen(currentTxt)*sizeof(char));
			}
			else{
				img->comments = (char*) realloc(img,
					(strlen(img->comments)+strlen(currentTxt))*sizeof(char));
			}
			strcpy(img->comments,currentTxt);
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
	nPix = (img->width)*(img->height);
	
	img->arrayRVB = (unsigned char*) malloc(nPix*3*sizeof(unsigned char));//1px = 3 composantes
	if(img->arrayRVB == NULL){
		free(img->comments);
		free(img);
	}
	
	
	//Lecture des pixels en binaire
	imgFile = fopen(fileName, "rb");
	fseek(imgFile,position, SEEK_SET);
	
	ctr = 0;
	
	/*On commence par le dernier pixel (image stockées 'à l'envers' avec le 
	haut de l'image en bas du tableau). On décale de trois pour prendre les composantes dans le bon ordre*/
	for(i =(nPix*3)-3; i >= 0; i=i-3){		
		readNUchar(&currentPix, 1, imgFile);
		img->arrayRVB[i] = currentPix;
		readNUchar(&currentPix, 1, imgFile);
		img->arrayRVB[i+1] = currentPix;
		readNUchar(&currentPix, 1, imgFile);
		img->arrayRVB[i+2] = currentPix;
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
	char sW[5]; char sH[5];
	short i = 0, j = 0;
	
	sW[4] = sH[4] = '\0';
	int sText = strlen(text);
	
	while(!isspace(text[i]) && i < 4){
		sW[i] = text[i];
		i++;
	}

	*w = atoi(sW);
	i++;
	while(text[i+j] != '\n' && j < 4 && (i+j) < sText){
		sH[j] = text[i+j];
		j++;
	}
	*h = atoi(sH);
}

void freeImage(Image* img){
	free(img->comments);
	free(img->arrayRVB);
	free(img);
}

bool saveImage(Image* img){
	FILE* imgFile = NULL;
	char format[4], dim[4+1+4], maxVal[4];
	long nPix,i;
	
	if(img == NULL || img->arrayRVB == NULL){
		fprintf(stderr,"Image nulle ou vide de pixels.\n");
		return false;
	}
	
	
	if(img->name == NULL){
		img->name = (char*) malloc((strlen("noname.ppm")+1)*sizeof(char));
		strcpy(img->name,"noname.ppm");
	}
	
	imgFile = fopen(img->name, "w");
	
	
	if(imgFile == NULL){
		fprintf(stderr,"Impossible de sauvegarder l'image. ");
		fprintf(stderr,"Création de %s impossible.\n", img->name);
		return false;
	}
	
	//Ecriture de l'en-tête
	
	/*
	 * while(fputs(...) == EOF) => tente d'écrire dans le fichier
	 * jusqu'à ce que fputs réussisse.
	 */
	
	sprintf(format,"P%d\n",img->format);
	while(fputs(format, imgFile) == EOF);
	
	sprintf(dim,"%d %d", img->width,img->height);
	while(fputs(dim, imgFile) == EOF);
	while(fputs("\n", imgFile) == EOF);
	
	sprintf(maxVal, "%d", img->maxValue);
	while(fputs(maxVal, imgFile) == EOF);
	while(fputs("\n", imgFile) == EOF);
	
	if(img->comments != NULL){
		fputs(img->comments,imgFile);
	}
	
	fflush(imgFile);
	fclose(imgFile);
	
	//Ecriture des pixels
	nPix = img->width*img->height;
	imgFile = fopen(img->name, "ab");
	if(imgFile == NULL){
		fprintf(stderr, "Impossible d'écrire les données binaires du fichier %s.\n", img->name);
		return false;
	}
	
	//On inverse le tableau avant de l'écrire
    	invertPPMArray(img->arrayRVB,nPix*3);
    	
	writeNUchar(img->arrayRVB, nPix*3, imgFile);
	
	//On remet le tableau dans le bon sens après l'avoir écrit
    	invertPPMArray(img->arrayRVB,nPix*3);
    	
	fflush(imgFile);
	
	fclose(imgFile);

	
	return true;
}

//Fonction de test à supprimer
void main(void){
	Image* test = loadImage("../images/Clown.256.ppm");
	
	strcpy(test->name, "../images/Clown.257.ppm");
	saveImage(test);
	
	freeImage(test);
	
	test = loadImage("../images/Clown.257.ppm");

	if(test)
		initGLIMAGIMP(test->width,test->height,test->arrayRVB);
	free(test);
	
}
