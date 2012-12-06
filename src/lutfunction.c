#include "lutfunction.h"
#include "lut.h"
#include <stdlib.h>
#include <stdio.h>

void invertLut(Lut* lt){
	int i=0;
	
	if(lt == NULL) return;
	
	for(i; i < lt->size; i++){
		lt->outputArrayRGB[i] = lt->maxValue - lt->inputArrayRGB[i];
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
		lt->outputArrayRGB[i]= lt->inputArrayRGB[i]+val;
		if (lt->outputArrayRGB[i]>lt->maxValue){
			lt->outputArrayRGB[i]=lt->maxValue;
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
		lt->outputArrayRGB[i]= lt->inputArrayRGB[i]-val;
		if (lt->outputArrayRGB[i]<0){
			lt->outputArrayRGB[i]=0;
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
		lt->outputArrayRGB[i]= (lt->inputArrayRGB[i]*coef)-diff;
		if (lt->outputArrayRGB[i]<0){
			lt->outputArrayRGB[i]=0;
			}
		if (lt->outputArrayRGB[i]>lt->maxValue){
			lt->outputArrayRGB[i]=lt->maxValue;
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
		lt->outputArrayRGB[i]= (lt->inputArrayRGB[i]*coef)-diff;
		if (lt->outputArrayRGB[i]<0){
			lt->outputArrayRGB[i]=0;
			}
		if (lt->outputArrayRGB[i]>lt->maxValue){
			lt->outputArrayRGB[i]=lt->maxValue;
			}
		}
}
/*
void sepia(Lut* lt){
	int i=0;
	if(lt == NULL) return;
	
	for (i;i<lt->size;i++){
	
	//to be continued
	}	
*/	
