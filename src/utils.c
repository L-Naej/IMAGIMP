#include "utils.h"

//Lit n unsigned char d'un fichier
int readNUchar(unsigned char* t, int n, FILE* f){
	int i, readed = 0;
	if(f==NULL)return 0;
	while(n!=readed && (i = fread(t+readed, n - readed, sizeof(unsigned char),f)) == 0){
		readed += i;
	}
	
	return(n == readed);
}
