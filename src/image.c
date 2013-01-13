#include "image.h"
#include "utils.h"
#include "lut.h"
#include <interface.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE_LENGTH 70
#define N_HEADER_DATALINE 3

Image* loadImage(char* fileName){
	if(fileName == NULL){
		fprintf(stderr, "Le nom du fichier image est vide.\n");
		return NULL;
	}
	
	FILE* imgFile = NULL;
	Image* img = NULL;
	
	char currentTxt[MAX_LINE_LENGTH];
	
	long position = 0;
	short ctr = 0;
	long int nPix;
	
	imgFile = fopen(fileName, "r");
	if(imgFile==NULL){
		fprintf(stderr, "Impossible d'ouvrir le fichier image \"%s\".\n",fileName);
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
				img->comments = (char*) malloc((strlen(currentTxt)+1)*sizeof(char));
			}
			else{
				img->comments = (char*) realloc(img->comments,
					(strlen(img->comments)+strlen(currentTxt)+1)*sizeof(char));
			}
			strcpy(img->comments,currentTxt);
			continue;	
		}
		
		switch(ctr){
			case 0 : img->format = (short) atoi((currentTxt+1));
			break;
			case 1 : 
				//detectWH(currentTxt, &img->width, &img->height);
				sscanf(currentTxt, "%d %d", &img->width, &img->height);
			break;
			case 2 : img->maxValue = atoi(currentTxt);
			break;
		}
		ctr++;
	}
	
	position = ftell(imgFile);
	
	fclose(imgFile);
	
	//Le programme ne gère que le PPM P6
	if(img->format != 6){
		fprintf(stderr, "Format de l'image non supporté (PPM P6 uniquement).\n");
		free(img->name);
		free(img->comments);
		free(img);
		return NULL;
	}
	
	//Allocation mémoire pour les lignes de pixels
	nPix = (img->width)*(img->height);
	
	img->arrayRGB = (unsigned char*) malloc(nPix*3*sizeof(unsigned char));//1px = 3 composantes
	if(img->arrayRGB == NULL){
		free(img->comments);
		free(img);
		return NULL;
	}
	
	
	//Lecture des pixels en binaire
	imgFile = fopen(fileName, "rb");
	fseek(imgFile,position, SEEK_SET);
	
	//readNUchar(img->arrayRGB, nPix*NB_COL_COMP, imgFile); //parfois bug...
	fread(img->arrayRGB, nPix*NB_COL_COMP, sizeof(unsigned char),imgFile);
	

	//On stocke l'image inversée en mémoire
	//pour qu'elle soit toute prête à être affichée
	//par OpenGL
	invertPPMArray(img->arrayRGB, img->width, img->height);
	fclose(imgFile);
	
	return img;
}


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


void freeImage(Image* img){
	if(img == NULL) return;
	free(img->comments);
	free(img->arrayRGB);
	free(img);
}

bool saveImage(Image* img){
	FILE* imgFile = NULL;
	//Format : P+num+\0; dim : largeur+ +hauteur+\0
	//Maxval : 999+\0
	char format[3], dim[4+1+4+1], maxVal[4];
	long nPix;
	
	if(img == NULL || img->arrayRGB == NULL){
		fprintf(stderr,"Image nulle ou vide de pixels.\n");
		return false;
	}
	
	
	if(img->name == NULL){
		img->name = (char*) malloc((strlen("./images/noname.ppm")+1)*sizeof(char));
		strcpy(img->name,"./images/noname.ppm");
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
	
	//Ecriture format
	sprintf(format,"P%d",img->format);
	while(fputs(format, imgFile) == EOF);
	while(fputs("\n", imgFile) == EOF);
	fflush(imgFile);
	
	//Ecriture taille
	sprintf(dim,"%d %d", img->width,img->height);
	while(fputs(dim, imgFile) == EOF);
	while(fputs("\n", imgFile) == EOF);
	fflush(imgFile);
	
	//Ecriture maxVal
	sprintf(maxVal, "%d", img->maxValue);
	while(fputs(maxVal, imgFile) == EOF);
	while(fputs("\n", imgFile) == EOF);
	fflush(imgFile);
	
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
    	invertPPMArray(img->arrayRGB,img->width, img->height);
    	
	writeNUchar(img->arrayRGB, nPix*NB_COL_COMP, imgFile);
	
	//On remet le tableau dans le bon sens après l'avoir écrit
    	invertPPMArray(img->arrayRGB,img->width, img->height);
    	
	fflush(imgFile);
	
	fclose(imgFile);

	
	return true;
}

Image* createEmptyImg(int w, int h, int maxValue){
	Image* img = (Image*) malloc(sizeof(Image));
	int i;
	
	if(img == NULL) return NULL;
	
	img->format = 6;//PPM P6
	img->name = NULL;
	img->comments = NULL;
	
	img->width = w;
	img->height = h;
	img->maxValue = maxValue;
	
	long int nPix = w*h*NB_COL_COMP;
	
	img->arrayRGB = (unsigned char*) calloc(nPix,sizeof(unsigned char));
	
	for(i=0; i < nPix; ++i){
		img->arrayRGB[i] = maxValue;
	}
	
	return img;
}

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

Image* copyImage(Image* source){
	if(source == NULL || source->arrayRGB == NULL) return NULL;
	
	Image* copy = createEmptyImg(source->width, source->height, source->maxValue);
	if(copy == NULL){
		fprintf(stderr, "Erreur mémoire lors de la copie d'une image.\n");
		return NULL;
	}
	int i = 0, size = 0, nPix = source->width*source->height*NB_COL_COMP;
	if(source->name != NULL){
		size = strlen(source->name) + 1;
		copy->name = (char*) calloc(size, sizeof(char));
		strcpy(copy->name, source->name);
	 }
	 else copy->name = NULL;
		
	copy->maxValue = source->maxValue;
	copy->width = source->width;
	copy->height = source->height;
	copy->format = source->format;
	
	for(i = 0; i < nPix; ++i){
		copy->arrayRGB[i] = source->arrayRGB[i];
	}
	
	return copy;
}

bool histoRGB (Image* img, int** hR,  int** hG,  int** hB){
	if (img == NULL) return false;
	if (hR==NULL || hG==NULL || hB==NULL) return false;
	 
	int i;
	int* histoR=(int*)calloc(256, sizeof(int));
	int* histoG=(int*)calloc(256, sizeof(int));
	int* histoB=(int*)calloc(256, sizeof(int));
	
	for (i=0; i <(img->width*img->height)*3; i+=3){ 
		histoR[img->arrayRGB[i]]=histoR[img->arrayRGB[i]]+1;
		histoG[img->arrayRGB[i+1]]=histoG[img->arrayRGB[i+1]]+1;
		histoB[img->arrayRGB[i+2]]=histoB[img->arrayRGB[i+2]]+1;
		}	
	*hR=histoR;
	*hG=histoG;
	*hB=histoB;
	return true;
}
	
bool histo (Image* img, long int** h){
	if(img ==NULL || h==NULL)return false;
	int i;
	int val=0;
	long int* histo=(long int*)calloc(img->maxValue+1, sizeof(long int));
	
	for (i=0; i <((img->width)*(img->height))*3; i+=3){ 
		val= (img->arrayRGB[i]+img->arrayRGB[i+1]+img->arrayRGB[i+2])/3;
		histo[val]++;
	}
	normalizeHisto(histo, img->maxValue + 1);	
	*h=histo;
	return true;
}

//L'histogramme donne des pourcentages
void normalizeHisto(long int* histo, int size){
	int i = 0;
	long int max = maxInTab(histo, size);
	for(i = 0; i < size; ++i){
		//Wouhou mini optimisation
		if(histo[i] == 0) continue;
		//Arrondi supérieur
		histo[i] = (long int) ceil(((double)((double)histo[i] / (double)max)) * 100.0);

	}
	
}

Image* createHistogram(Image* source){
	if(source == NULL) return NULL;
	Image* histogram = NULL;
	long int* tabHisto = NULL;
	
	int i = 0, j = 0, k = 0, x = 0, y = 0, index = 0;
	//Largeur en pixel d'une colonne de valeur de l'histo
	int wCol = ((int) floor(source->width / (source->maxValue + 1)) );
	//Hauteur en pixel d'une unité de l'axe Y (un % de l'histo)
	int hCol = ((int) floor(source->height / 100) );
	
	//Calcul du tableau représentant l'histogramme
	histo(source, &tabHisto);
	if(tabHisto == NULL){
		fprintf(stderr, "Le calcul de l'histogramme à échoué.\n");
		return NULL;
	}
	
	//Génération de l'image qui va représenter graphiquement l'histogramme
	histogram = createEmptyImg(source->width, source->height, source->maxValue);
	if(histogram == NULL){
		fprintf(stderr, "Impossible d'allouer la mémoire nécessaire pour l'histogramme de l'image %s.\n", source->name==NULL? "(nom inconnu)" : source->name);
		free(tabHisto);
		return NULL;
	}
	
	//Remplissage de l'image
	/*
	 * Pour les deux premières boucles for on raisonne en pixels.
	 * Les deux dernières boucles for aussi mais index
	 * raisonne en 3 composantes par pixels.
	 */
	for(y = 0; y + hCol <= source->height; y = y + hCol){
		for(x = 0; x + wCol <= source->width; x = x + wCol){
			k = x / wCol;
			if ( 100 - (y/hCol) < tabHisto[k] && tabHisto[k] != 0){
				for(i = y; i < y + hCol; ++i){
					for(j = x; j < x + wCol; ++j){
						index = i*(source->width)*3 + j*3;
						histogram->arrayRGB[index] = 0;
						histogram->arrayRGB[index+1] = 0;
						histogram->arrayRGB[index+2] = 0;
					}
				}
			}
		}
	}
	
	invertPPMArray(histogram->arrayRGB, histogram->width, histogram->height);
	
	return histogram;
}
	
void dumpImage(Image* img){
	if(img == NULL){
		printf("Tentative de dump d'une image NULL.\n");
		return;	
	} 
	printf("--Image (Adresse %p)\n", img);
	if(img->name) printf("Nom : %s\n", img->name);
	if(img->comments) printf("Commentaires : %s\n", img->comments);
	printf("Width : %d\nHeight : %d\n", img->width, img->height);
	
}


