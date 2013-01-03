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
		lt->channels->chan1[i] = tmp;
		
		tmp = input->chan2[i]+lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan2[i] = tmp;
		
		tmp = input->chan3[i]+lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan3[i] = tmp;
		
		
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
		lt->channels->chan1[i] = tmp;
		
		tmp = input->chan2[i] - lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan2[i] = tmp;
		
		tmp = input->chan3[i] - lt->valueEffect;
		if(tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan3[i] = tmp;

	}
}

/*Valeur de val comprises entre 1 et 100 pour une diminution*/

void dimContrast(Lut* lt, Channels* input){
	int i,tmp;
	
	if(lt == NULL || lt->valueEffect<=0 || lt->valueEffect>=100) return;
	
	for (i=0;i<lt->size;i++){
		tmp=128+(input->chan1[i]-128)*lt->valueEffect/100;
		if (tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan1[i] = tmp;
		
		tmp=128+(input->chan2[i]-128)*lt->valueEffect/100;
		if (tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan2[i] = tmp;
		
		tmp=128+(input->chan3[i]-128)*lt->valueEffect/100;
		if (tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan3[i] = tmp;
	}
}

/*Valeur de val comprises entre 100 et 200 pour une augmentation*/

void addContrast(Lut* lt, Channels* input){
	int i,tmp;
	
	if(lt == NULL || lt->valueEffect<=100 || lt->valueEffect>=200) return;
	
	for (i=0;i<lt->size;i++){
		tmp = 128 + (input->chan1[i]-128) * (lt->valueEffect)/100;
		if (tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan1[i] = tmp;
		
		tmp = 128 + (input->chan2[i]-128) * (lt->valueEffect)/100;
		if (tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan2[i] = tmp;
		
		tmp = 128 + (input->chan3[i]-128) * (lt->valueEffect)/100;
		if (tmp > lt->maxValue) tmp = lt->maxValue;
		else if (tmp < 0) tmp = 0;
		lt->channels->chan3[i] = tmp;
	}
}

/*
void sepia(Lut* lt){
	int i;
	if(lt == NULL) return;
	for(i=0; i < lt->size; i++){
		lt->outputArrayR[i] = (lt->inputArrayR[i]* .393) + (lt->inputArrayG[i]*.769) + (lt->inputArrayB[i] * .189);

		lt->outputArrayG[i] = (lt->inputArrayR[i] * .349) + (lt->inputArrayG[i] *.686) + (lt->inputArrayB[i] * .168);

		lt->outputArrayB[i] = (lt->inputArrayR[i] * .272) + (lt->inputArrayG[i] *.534) + (lt->inputArrayB[i] * .131);
		}
}
*/
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
