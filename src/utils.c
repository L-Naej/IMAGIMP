#include "utils.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define NB_COL_COMP 3
#define F_NAME_PER_LINE 3

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
bool writeNUchar(const unsigned char* t, int n, FILE* f){
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

//credit: 
//http://www.gnu.org/software/libtool/manual/libc/Simple-Directory-Lister.html
//Légèrement modifiée pour affichage et pour
//renvoyer la taille du plus long nom de fichier
int printDirectory(const char dirName[]){
	if(dirName == NULL) return 0;
	int cnt = 0, nameSize;
	int maxNameLenght = 0;

	DIR *dp;
	struct dirent *ep;     
	dp = opendir (dirName);
	
	printf("\n\n\t");
	if (dp != NULL)
	{
		while ( ( ep = readdir (dp) ) ){
			//On ne veut afficher ni le dossier courant
			//ni le dossier parent 
			if(strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
				continue;
			cnt++;
			fputs(ep->d_name, stdout);
			if( ( nameSize = strlen(ep->d_name) ) > maxNameLenght)
				maxNameLenght = nameSize;
				
			printf(" | ");
			if(cnt == F_NAME_PER_LINE){
				printf("\n\t");
				cnt = 0;
			} 
		}	
		printf("\n\n");
		(void) closedir (dp);
	}
	else
		perror ("Couldn't open the directory");
	return maxNameLenght;
}

//FAQ developpez.com
char * readStdin(char * buffer, size_t taille){
	char * result = fgets(buffer, taille, stdin);

	if (result != NULL){
		char * lf = strchr(buffer, '\n'); /* On cherche le caractere '\n'. */
		if (lf != NULL) /* S'il est present, ... */
			*lf = '\0'; /* ... on le supprime    */
		else
			clearStdin();
		
	}
	
	return result;
}

//FAQ developpez.com
void clearStdin(){
	char c;
	do {
	  c = getchar();
	} while (c != '\n' && c != EOF);
}

unsigned char minInTab(unsigned char* tab, int size){
	if(tab == NULL) return 0;
	
	int i = 0, min = 255;//taille max d'un unsigned char
	
	for(i = 0; i < size; ++i){
		if(tab[i] < min)
			min = tab[i];
	}
	
	return min;
}

unsigned char maxInTab(unsigned char* tab, int size){
	if(tab == NULL) return 0;
	
	int i = 0, max = 0;//taille min d'un unsigned char
	
	for(i = 0; i < size; ++i){
		if(tab[i] > max)
			max = tab[i];
	}
	
	return max;
}
