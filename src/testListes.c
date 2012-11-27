#include "testListes.h"
#include "layer.h"
#include "image.h"
#include <stdlib.h>

//Variables globales de test
Image* test ;
Layer* l1, *l2, *l3, *l4, *lInsert1, *lInsert2,*lInsert3, *lInsert4, *lInsert5, *lInsert6, *lInsert7, *lInsert8 ;
LayerList* ll1, *ll2;

void main(void){
	test = loadImage("../images/Clown.256.ppm");

	l1 = createLayer(test, 0.5, SUM);
	l2 = createLayer(test, 0.1, MULTIPLICATION);
	l3 = createLayer(test, 1.0, SUM);
	l4 = createLayer(test, 0.8, SUM);
	
	//On connecte à la main
	l1->nextLayer = l2;
	l2->previousLayer = l1;
	l2->nextLayer = l3;
	l3->previousLayer = l2;
	l3->nextLayer = l4;
	l4->previousLayer = l3;
	
	//Ceux là ne seront pas connectés mais rajoutés à la main dans la liste
	lInsert1 = createLayer(test, 0.8, SUM);
	lInsert2 = createLayer(test, 0.8, SUM);
	lInsert3 = createLayer(test, 0.4, SUM);
	lInsert4 = createLayer(test, 0.8, SUM);
	lInsert5 = createLayer(test, 0.8, SUM);
	lInsert6 = createLayer(test, 0.8, SUM);
	lInsert7 = createLayer(test, 0.8, SUM);
	lInsert8 = createLayer(test, 0.8, SUM);

	ll1 = NULL;
	ll2 = NULL;
	testCreate();
	testGoCurrentNextPreviousHeadBottom();
	testInsert();
	
	testPosition();//En cours

}

void testCreate(){
	//Test création
	ll1 = createLayerList(l1);
	ll2 = createEmptyLayerList();
	
	dumpLayerList(ll1);
	dumpLayerList(ll2);
}

void testGoCurrentNextPreviousHeadBottom(){
	
	fprintf(stderr, "Test goToHead\n");
	goToHeadLayer(ll1);
	if(ll1->cursor != NULL){
		fprintf(stderr, "Test goToHead failed.\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test previousLayer from head\n");
	if(previousLayer(ll1) != NULL){
		fprintf(stderr,"Test previousLayer from head failed.\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test currentLayer from head.\n");
	if(currentLayer(ll1) != NULL){
		fprintf(stderr, "Test currentLayer from head failed.\n");
		exit(-1);
	
	}

	fprintf(stderr,"Test nextLayer from head.\n");
	if(nextLayer(ll1) != l1){
		fprintf(stderr,"Test nextLayer from head failed.\n");
		exit(-1);
	}
	
	fprintf(stderr,"Test previousLayer from first position.\n");
	if(previousLayer(ll1) != NULL){
		fprintf(stderr,"Test previousLayer from first position failed.\n");
		exit(-1);
	}
	
	ll1->cursor = l1;
	
	fprintf(stderr, "Test normal currentLayer.\n");
	if(currentLayer(ll1) != l1){
		fprintf(stderr, "Test normal currentLayer failed.\n");
		exit(-1);
	
	}
	
	fprintf(stderr,"Test normal nextLayer.\n");
	if(nextLayer(ll1) != l2){
		fprintf(stderr,"Test normal nextLayer failed.\n");
		exit(-1);		
	}
	
	fprintf(stderr,"Test normal previousLayer.\n");
	if(previousLayer(ll1) != l1){
		fprintf(stderr,"Test normal previousLayer failed.\n");
		exit(-1);
	}
	
	goToBottomLayer(ll1);

	fprintf(stderr,"Test goToBottom.\n");
	dumpLayer(ll1->cursor);
	if(ll1->cursor != l4){
		fprintf(stderr,"Test goToBottom failed.\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test currentLayer from bottom.\n");
	if(currentLayer(ll1) != l4){
		fprintf(stderr, "Test currentLayer from bottom failed.\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test nextLayer from bottom.\n");
	if(nextLayer(ll1) != NULL){
		fprintf(stderr, "Test nextLayer from bottom failed.\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test previousLayer from bottom.\n");
	if(previousLayer(ll1) != l3){
		fprintf(stderr, "Test previousLayer from bottom failed.\n");
		exit(-1);
	}	
}	
	
void testInsert(){
	fprintf(stderr,"Test InsertHead\n");
	
	insertHeadLayer(ll1, lInsert1);
	
	if(ll1->head != lInsert1){
		fprintf(stderr, "Test insertHeadLayer failed\n");
		exit(-1);
	}
	
	fprintf(stderr,"Test insertbottom\n");
	
	insertBottomLayer(ll1,lInsert2);
	
	if(ll1->bottom != lInsert2 || l4->nextLayer != lInsert2 || lInsert2->previousLayer != l4){
		fprintf(stderr, "Test insertbottom failed\n");
		exit(-1);
	}
	
	goToBottomLayer(ll1);
	
	fprintf(stderr, "Test insertAfterLayer cursor on bottom\n");
	insertAfterLayer(ll1, lInsert3);
	if(ll1->bottom != lInsert3 || lInsert2->nextLayer != lInsert3 || lInsert3->previousLayer != lInsert2 ){
		fprintf(stderr, "Test insertAfterLayer cursor on bottom failed\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test insertAfterLayer cursor normal\n");
	ll1->cursor = l2;
	
	insertAfterLayer(ll1, lInsert4);
	if(nextLayer(ll1) != lInsert4 || lInsert4->previousLayer != l2 || l2->nextLayer != lInsert4){
		fprintf(stderr, "Test insertAfterLayer cursor normal failed\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test insertAfterLayer cursor on head\n");
	goToHeadLayer(ll1);

	insertAfterLayer(ll1, lInsert5);
	if(nextLayer(ll1) != lInsert5 || lInsert5->nextLayer != lInsert1 || lInsert1->previousLayer != lInsert5 ){
		fprintf(stderr, "Test insertAfterLayer cursor on head failed\n");
		exit(-1);
	}

	fprintf(stderr, "Test insertBeforeLayer cursor on head\n");
	
	goToHeadLayer(ll1);
	insertBeforeLayer(ll1, lInsert6);
	
	if(nextLayer(ll1) != lInsert6 || lInsert6->nextLayer != lInsert5 || lInsert5->previousLayer != lInsert6){
		fprintf(stderr,"Test insertBeforeLayer cursor on head failed\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test insertBeforeLayer cursor normal \n");
	
	ll1->cursor = l3;
	insertBeforeLayer(ll1, lInsert7);
	
	if(previousLayer(ll1) != lInsert7 || lInsert7->nextLayer != l3 || l3->previousLayer != lInsert7){
		fprintf(stderr,"Test insertBeforeLayer cursor normal failed\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test insertBeforeLayer cursor on bottom\n");
	
	goToBottomLayer(ll1);
	insertBeforeLayer(ll1, lInsert8);
	
	if(previousLayer(ll1) != lInsert8 || lInsert8->nextLayer != ll1->bottom || ll1->bottom->previousLayer != lInsert8){
		fprintf(stderr,"Test insertBeforeLayer cursor on bottom failed\n");
		exit(-1);
	}
}

void testPosition(){
	
	fprintf(stderr, "Test getLayerByPosition\n");
	
	if(getLayerByPosition(ll1,1) != ll1->head){
		fprintf(stderr, "Test getLayerByPosition (pos 1) failed.\n");
		exit(-1);	
	}
	
	if(getLayerByPosition(ll1, 12) != goToBottomLayer(ll1)){
		fprintf(stderr, "Test getLayerByPosition (pos 12 (bottom)) failed.\n");
		exit(-1);
	}
	
	if(getLayerByPosition(ll1, 4) != l1){
		fprintf(stderr, "Test getLayerByPosition (pos 4 (random position)) failed.\n");
		exit(-1);
	}
	
	fprintf(stderr, "Test delLayerByPosition\n");
	int initSize = ll1->nbLayers;
	if(delLayerByPosition(ll1, 1) != lInsert6 || ll1->head == lInsert6 || ll1->head != lInsert5 || ll1->nbLayers != (initSize-1)){
		fprintf(stderr, "Test delLayerByPosition (head) failed\n");
		exit(-1);
	}
	

	if(delLayerByPosition(ll1, ll1->nbLayers) != lInsert8 || ll1->bottom == lInsert8 || ll1->bottom != lInsert2){
		fprintf(stderr, "Test delLayerByPosition (bottom) failed\n");
		exit(-1);
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

