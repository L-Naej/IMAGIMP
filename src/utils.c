#include "utils.h"
#include <stdlib.h>
#include <string.h>

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

//Inverse un tableau au format ppm (RVB unsigned char)
void invertPPMArray(unsigned char array[], long size){
      long i = 0;
     	unsigned char* invertedArray = (unsigned char*) malloc(size*sizeof(unsigned char));
      
      for(i = size-3; i >= 0; i=i-3){		
		
		invertedArray[size-1-i-2] = array[i];
		
		invertedArray[size-1-i-1] = array[i+1];
		
		invertedArray[size-1-i] = array[i+2];
	}
	
	memcpy(array, invertedArray, size*sizeof(unsigned char));
}
