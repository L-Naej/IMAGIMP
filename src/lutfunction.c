#include "lutfunction.h"
#include "lut.h"
#include <stdlib.h>
#include <stdio.h>


void neutral(Lut* lt){
	int i;
	
	if(lt == NULL) return;
	
	for(i=0; i < lt->size; i++){
		lt->outputArrayRGB[i] = lt->inputArrayRGB[i];
	}
}
	
void invertLut(Lut* lt){
	int i;
	
	if(lt == NULL) return;
	
	for(i=0; i < lt->size; i++){
		lt->outputArrayRGB[i] = lt->maxValue - lt->inputArrayRGB[i];
	}
}
	
void addLum(Lut* lt, int val){
	int i;
	int tmp;
	
	if(lt == NULL) return;
	
	if (val<0){
		printf("Value Error");
		return;
		}		
	for (i=0;i<lt->size;i++){
		tmp=lt->inputArrayRGB[i]+val;
		
		if (tmp<=255)lt->outputArrayRGB[i]=tmp;
		else	lt->outputArrayRGB[i]=255;
		
		}
}
	
void dimLum(Lut* lt, int val){
	int i;
	int tmp;
	
	if(lt == NULL) return;
	
	if (val>255){
		printf("Value Error");
		return;
		}
		
	for (i=0;i<lt->size;i++){
		tmp=lt->inputArrayRGB[i]-val;
		
		if (tmp>=0)lt->outputArrayRGB[i]=tmp;
		else lt->outputArrayRGB[i]=0;
		printf("%d \n",lt->outputArrayRGB[i]);
		}
}

/*Valeur de val comprises entre 1 et 100 pour une diminution*/
void dimContrast(Lut* lt, int val){
	int i,tmp;
	
	if(lt == NULL || val<=0 || val>=100) return;
	
	for (i=0;i<lt->size;i++){
		tmp=128+(lt->inputArrayRGB[i]-128)*val/100;
		if (tmp>0 && tmp<lt->maxValue) lt->outputArrayRGB[i]=tmp;
		else if (tmp<0)lt->outputArrayRGB[i]=0;
		else lt->outputArrayRGB[i]=lt->maxValue;
		}
}

/*Valeur de val comprises entre 100 et 200 pour une augmentation*/
void addContrast(Lut* lt, int val){
	int i,tmp;
	
	if(lt == NULL || val<=100 || val>=200) return;
	
	for (i=0;i<lt->size;i++){
		tmp=128+(lt->inputArrayRGB[i]-128)*val/100;
		if (tmp>0 && tmp<lt->maxValue) lt->outputArrayRGB[i]=tmp;
		else if (tmp<0)lt->outputArrayRGB[i]=0;
		else lt->outputArrayRGB[i]=lt->maxValue;
		}
}

void sepia(Lut* lt){
	int i;
	if(lt == NULL) return;
	for(i=0; i < lt->size; i++){
		lt->outputArrayR[i] = (lt->inputArrayR[i]* .393) + (lt->inputArrayG[i]*.769) + (lt->inputArrayB[i] * .189);

		lt->outputArrayG[i] = (lt->inputArrayR[i] * .349) + (lt->inputArrayG[i] *.686) + (lt->inputArrayB[i] * .168);

		lt->outputArrayB[i] = (lt->inputArrayR[i] * .272) + (lt->inputArrayG[i] *.534) + (lt->inputArrayB[i] * .131);
		}
}		
