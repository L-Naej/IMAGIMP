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

/*Valeur de val comprises entre 100 et 200 pour une augmentation*/
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


void sepia(Lut* lt, Channels* input){
	int i;
	if(lt == NULL) return;
	for(i=0; i < lt->size; i++){
		lt->channels->chan1[i] = (unsigned char) floor( (input->chan1[i]* .393) + (input->chan2[i]*.769) + (input->chan3[i] * .189) + 0.5);

		lt->channels->chan2[i] = (unsigned char) floor( (input->chan1[i] * .349) + (input->chan2[i] *.686) + (input->chan3[i] * .168) + 0.5);

		lt->channels->chan3[i] = (unsigned char) floor( (input->chan1[i] * .272) + (input->chan2[i] *.534) + (input->chan3[i] * .131) + 0.5);
	}
}

//Formule trouvée sur http://www.fourcc.org/fccyvrgb.php
//in[0] = R; in[1] = G; in[2] = B
//out[0] = Y; out[1] = U; out[2] = V
Channels* rgbToYUV(Channels* input, Channels* output, int size, int maxValue){
	if(input == NULL || output == NULL) return NULL;
	int tmp, i = 0;
	
	for ( i = 0; i < size; ++i ){
		
		//Y = 0.299R + 0.587G + 0.114B
		//tmp = (int) floor(( 0.299 * input->chan1[i]) + (0.587 * input->chan2[i]) + (0.114 * input->chan3[i]) + 0.5 );
		//Y  =      (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
		tmp = (int) floor( 0.257*input->chan1[i] + 0.504*input->chan2[i] + 0.098*input->chan3[i] + 16 + 0.5);
		if(tmp > maxValue) tmp = maxValue;
		if(tmp < 0) tmp = 0;
		output->chan1[i] = (unsigned char) tmp;
		
		//U= (B-Y)*0.565
		//tmp = (int) floor( input->chan3[i] - output->chan1[i]*0.565 + 0.5);
		//U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128
		tmp = (int) floor( -(0.148*input->chan1[i]) + (0.294*input->chan2[i]) + (0.439*input->chan3[i]) + 128 + 0.5);
		if(tmp > maxValue) tmp = maxValue;
		if(tmp < 0) tmp = 0;
		output->chan2[i] = (unsigned char) tmp;
		
		//V= (R-Y)*0.713
		//tmp = (int) floor( input->chan1[i] - output->chan1[i]*0.713 +0.5);
		//V =  (0.439 * R) - (0.368 * G) - (0.071 * B) + 128
		tmp = (int) floor( 0.439*input->chan1[i] - (0.368*input->chan2[i]) - (0.071*input->chan3[i]) + 128 + 0.5 );
		if(tmp > maxValue) tmp = maxValue;
		if(tmp < 0) tmp = 0;
		output->chan3[i] = (unsigned char) tmp;
	}
	
	return output;
}

//Formule trouvée sur http://www.fourcc.org/fccyvrgb.php
//in[0] = Y; in[1] = U; in[2] = V
//out[0] = R; out[1] = G; out[2] = B
Channels* yuvToRGB(Channels* input, Channels* output, int size, int maxValue){
	if(input == NULL || output == NULL) return NULL;
	
	int tmp, i = 0;
	
	for(i=0; i < size; ++i){
		//R = Y + 1.403V
		//tmp = (int) floor(input->chan1[i] + 1.403*input->chan3[i] + 0.5) ;
		//R  = 1.164(Y - 16) + 1.596(V - 128)
		tmp = (int) floor( 1.164*(input->chan1[i] - 16) + 1.596*(input->chan3[i] - 128) + 0.5);
		if(tmp > maxValue) tmp = maxValue;
		if(tmp < 0) tmp = 0;
		output->chan1[i] = (unsigned char) tmp;
		
		//G = Y - 0.344U' - 0.714V'
		//tmp = (int) floor( input->chan1[i] - 0.344*input->chan2[i] - 0.714*input->chan3[i]+ 0.5);
		//G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
		tmp = (int) floor( 1.164*(input->chan1[i] - 16) - 0.813*(input->chan3[i] - 128) - 0.391*(input->chan2[i] - 128) + 0.5);
		if(tmp > maxValue) tmp = maxValue;
		if(tmp < 0) tmp = 0;
		output->chan2[i] = (unsigned char) tmp;
		
		//B = Y + 1.770U'
		//tmp = (int) floor(input->chan1[i] + 1.770*input->chan2[i] + 0.5);
		//B = 1.164(Y - 16)                   + 2.018(U - 128)
		tmp = (int) floor( 1.164*(input->chan1[i] - 16) + 2.018*(input->chan2[i] - 128) + 0.5);
		if(tmp > maxValue) tmp = maxValue;
		if(tmp < 0) tmp = 0;
		output->chan3[i] = (unsigned char) tmp;
	}
	return output;
}
