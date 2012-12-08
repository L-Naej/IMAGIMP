#include "utils.h"
#include "image.h"
#include <stdlib.h>
#include <string.h>
#define NB_COL_COMP 3
//Lit n unsigned char d'un fichier
bool readNUchar(unsigned char* t, int n, FILE* f){
	int i, readed = 0;
	if(f==NULL)return 0;
	while(n!=readed && (i = fread(t+readed, n - readed, sizeof(unsigned char),f)) == 0){
		readed += i;
	}
	
	return (n==readed);
}

//Ecrit n unsigned char dans un fichier
bool writeNUchar(unsigned char* t, int n, FILE* f){
	int i=0, writed = 0;
	if(f==NULL)return 0;
	
	while(n!=writed && i  == 0){
		i = fwrite(t+writed, n - writed, sizeof(unsigned char),f);
		writed += i;
	}
	
	return(n == writed);
}

//Inverse un tableau au format ppm (RGB unsigned char)
void invertPPMArray(unsigned char array[], long int width, long int height){
      long i = 0, j= 0;
      long int nPix = width*height;
     	unsigned char* invertedArray = (unsigned char*) malloc(nPix*NB_COL_COMP*sizeof(unsigned char));
      
      int k = 0;
      
	for(i = height*(width-1)*NB_COL_COMP; i >= 0; i = i - (width*NB_COL_COMP)){
		for(j = 0; j < width*NB_COL_COMP; j++){
			invertedArray[k++] = array[i+j];
		}
	}
	
	memcpy(array, invertedArray, nPix*NB_COL_COMP*sizeof(unsigned char));
}
