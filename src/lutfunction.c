#include "lutfunction.h"
#include "lut.h"
#include <stdlib.h>
#include <stdio.h>

void invertLut(Lut* lt){
	int i=0;
	
	if(lt == NULL) return;
	
	for(i; i < lt->size; i++){
		lt->outputArrayRVB[i] = lt->maxValue - lt->inputArrayRVB[i];
	}
}
	
void addLum(Lut* lt, int val){
	int i=0;
	
	if(lt == NULL) return;
	
	if (val<0){
		printf("Value Error");
		return;
		}
		
	for (i;i<lt->size;i++){
		lt->outputArrayRVB[i]= lt->inputArrayRVB[i]+val;
		if (lt->outputArrayRVB[i]>255){
			lt->outputArrayRVB[i]=255;
			}
		}
}
	
void dimLum(Lut* lt, int val){
	int i=0;
	
	if(lt == NULL) return;
	
	if (val>255){
		printf("Value Error");
		return;
		}
		
	for (i;i<lt->size;i++){
		lt->outputArrayRVB[i]= lt->inputArrayRVB[i]-val;
		if (lt->outputArrayRVB[i]<0){
			lt->outputArrayRVB[i]=0;
			}
		}
}

void addContrast(Lut* lt, int val){
	int i=0,diff=0,coef=0;
	
	if(lt == NULL) return;
	
	if((val>=0) && (val<100)){
		coef= (val+1)/100;
		}
	else{
		coef=1;
		printf("Value Error");
		}
	
	diff=(128*coef)-128; 
	
	for (i;i<lt->size;i++){
		lt->outputArrayRVB[i]= (lt->inputArrayRVB[i]*coef)-diff;
		if (lt->outputArrayRVB[i]<0){
			lt->outputArrayRVB[i]=0;
			}
		if (lt->outputArrayRVB[i]>255){
			lt->outputArrayRVB[i]=255;
			}
		}
}

void dimContrast(Lut* lt, int val){
	int i=0,diff=0,coef=0;
	
	if(lt == NULL) return;
	
	if((val>0) && (val<=100)){
		coef= (101-val)/100;
		}
	else{
		coef=1;
		}
	
	diff=(128*coef)-128; 
	
	for (i;i<lt->size;i++){
		lt->outputArrayRVB[i]= (lt->inputArrayRVB[i]*coef)-diff;
		if (lt->outputArrayRVB[i]<0){
			lt->outputArrayRVB[i]=0;
			}
		if (lt->outputArrayRVB[i]>255){
			lt->outputArrayRVB[i]=255;
			}
		}
}
/*	
void sepia(Lut* lt){
	int i=0;
	int channel=0; // Va permettre de switcher la channel (sépia est la seule LUT nécessitant de prendre en compte chaque composante
	if(lt == NULL) return;
	
	//to be continued
	}
*/	
	
