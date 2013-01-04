#include "lutfunction.h"
#include "lut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void neutral(Lut* lt, Channels* input){
	int i;
	
	if(lt == NULL) return;
	
	for(i=0; i < lt->size; i++){
		lt->channels->chan1[i] = input->chan1[i];
		lt->channels->chan2[i] = input->chan2[i];
		lt->channels->chan3[i] = input->chan3[i];
	}
}
	
void invertLut(Lut* lt, Channels* input){
	int i;
	
	if(lt == NULL) return;
	
	for(i=0; i < lt->size; i++){
		lt->channels->chan1[i] = lt->maxValue - input->chan1[i];
		lt->channels->chan2[i] = lt->maxValue - input->chan2[i];
		lt->channels->chan3[i] = lt->maxValue - input->chan3[i];
	}
}

void addLum(Lut* lt, Channels* input){
	int i;
	int tmp;
	
	if(lt == NULL) return;
	/*
	Channels* yuv = allocChannels(lt->size);
	if(yuv == NULL) return;
	
	rgbToYUV(input, yuv, lt->size, lt->maxValue );
	*/
	for (i=0; i < lt->size; i++){
		
		tmp = input->chan1[i]+lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan1[i] = (unsigned char) tmp;
		
		tmp = input->chan2[i]+lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan2[i] = (unsigned char) tmp;
		
		tmp = input->chan3[i]+lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan3[i] = (unsigned char) tmp;
		
		
		/*
		tmp = yuv->chan1[i] + lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		yuv->chan1[i] = tmp;
		*/
	}
	
	//yuvToRGB(yuv, lt->channels, lt->size, lt->maxValue);
	//freeChannels(yuv);
}


void dimLum(Lut* lt, Channels* input){
	int i;
	int tmp;
	
	if(lt == NULL) return;
		
	for (i=0; i < lt->size; i++){
		tmp = input->chan1[i] - lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan1[i] = (unsigned char) tmp;
		
		tmp = input->chan2[i] - lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan2[i] = (unsigned char) tmp;
		
		tmp = input->chan3[i] - lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan3[i] = (unsigned char) tmp;

	}
}


/*Algo
	rg := (Abs(127 - r) * Cont) Div 255;
      gg := (Abs(127 - g) * Cont) Div 255;
      bg := (Abs(127 - b) * Cont) Div 255;
      If (r > 127) Then
        r := r + rg
      Else
        r := r - rg;
      If (g > 127) Then
        g := g + gg
      Else
        g := g - gg;
      If (b > 127) Then
        b := b + bg
      Else
        b := b - bg;
*/

void changeContrast(Lut* lt, Channels* input){

	int i = 0, rg, gg, bg;
	for(i = 0; i < lt->size; i++){
		rg = (abs(127 - input->chan1[i]) * lt->valueEffect) / lt->maxValue;
		if(input->chan1[i] > 127)
			rg = input->chan1[i] + rg;
		else
			rg = input->chan1[i] - rg;
		if(rg > lt->maxValue) rg = lt->maxValue;
		else if (rg < 0) rg = 0;
		
		lt->channels->chan1[i] = (unsigned char) rg;
		
		gg = (abs(127 - input->chan2[i]) * lt->valueEffect) / lt->maxValue;
		if(input->chan2[i] > 127)
			gg = input->chan2[i] + gg;
		else
			gg = input->chan2[i] - gg;
		if(gg > lt->maxValue) gg = lt->maxValue;
		else if (gg < 0) gg = 0;
		
		lt->channels->chan2[i] = (unsigned char) gg;
		
		bg = (abs(127 - input->chan3[i]) * lt->valueEffect) / lt->maxValue;
		if(input->chan3[i] > 127)
			bg = input->chan3[i] + bg;
		else
			bg = input->chan3[i] - bg;
		if(bg > lt->maxValue) bg = lt->maxValue;
		else if (bg < 0) bg = 0;
		
		lt->channels->chan3[i] = (unsigned char) bg;
	}
}


