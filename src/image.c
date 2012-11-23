#include "image.h"
#include "utils.h"
#include <interface.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 70
#define N_HEADER_DATALINE 3
#define DEFAULT_MAX_VAL 255
#define NB_COL_COMP 3 //nombre de composantes couleur (RVB=>3)

struct image{
	short format;//P1 to P6
	char* comments;
	char* name;
	int width;
	int height;
	int maxValue;
	unsigned char* arrayRVB;
	
};

//Charge une image depuis le disque dur
Image* loadImage(char* fileName){
	if(fileName == NULL){
		fprintf(stderr, "Le nom du fichier image est vide.\n");
		return NULL;
	}
	
	FILE* imgFile = NULL;
	Image* img = NULL;
	
	char currentTxt[MAX_LINE_LENGTH];
	unsigned char currentPix;
	
	long position = 0;
	short ctr = 0;
	long int i, nPix;
	
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
	for(i =(nPix*NB_COL_COMP)-NB_COL_COMP; i >= 0; i=i-NB_COL_COMP){		
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
	//On considère que largeur et hauteur max = 9999 donc 4 caractères + \0
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

//Libère une image en mémoire pointée par img
void freeImage(Image* img){
	free(img->comments);
	free(img->arrayRVB);
	free(img);
}

//Sauvegarde une image sur le disque dur
bool saveImage(Image* img){
	FILE* imgFile = NULL;
	//Format : P+num+\n+\0; dim : largeur+ +hauteur+\0
	//Maxval : 999+\0
	char format[4], dim[4+1+4+1], maxVal[4];
	long nPix;
	
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
    	invertPPMArray(img->arrayRVB,nPix*NB_COL_COMP);
    	
	writeNUchar(img->arrayRVB, nPix*NB_COL_COMP, imgFile);
	
	//On remet le tableau dans le bon sens après l'avoir écrit
    	invertPPMArray(img->arrayRVB,nPix*NB_COL_COMP);
    	
	fflush(imgFile);
	
	fclose(imgFile);

	
	return true;
}

//Créé une image vide noire (tous les pixels à 0) de dimension w*h
Image* createEmptyImg(int w, int h){
	Image* img = (Image*) malloc(sizeof(Image));
	
	if(img == NULL) return NULL;
	
	img->name = NULL;
	img->comments = NULL;
	
	img->width = w;
	img->height = h;
	img->maxValue = DEFAULT_MAX_VAL;
	
	long int nPix = w*h*NB_COL_COMP;
	
	img->arrayRVB = (unsigned char*) calloc(nPix,sizeof(unsigned char));
	
	return img;
}

//Ajoute ou modifie le nom d'une struct Image
bool imgAddName(Image* img, const char name[]){
	//strlen ne compte pas \0...
	int size = strlen(name)+1;
	//Au cas où l'allocation de nom échoue pour garder l'ancien
	char* tPtr = NULL;
	
	if(img == NULL || name == NULL) return false;
	

	tPtr = (char*) malloc(size*sizeof(char));
	if(tPtr == NULL) return false;
	
	free(img->name);
	
	img->name = tPtr;
	strcpy(img->name, name);
	
	return true;
}

//Fonction de test à supprimer
int main(int argc, char** argv){
	Image* test = loadImage("../images/Clown.256.ppm");
	
	strcpy(test->name, "../images/Clown.257.ppm");
	saveImage(test);
	
	freeImage(test);
	
	test = loadImage("../images/Clown.257.ppm");

	if(test)
		initGLIMAGIMP(test->width,test->height,test->arrayRVB);
	free(test);
	
	return 0;
}
