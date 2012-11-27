#include "testListes.h"
#include "layer.h"
#include "image.h"
#include <stdlib.h>


void main(void){
	Image* test = loadImage("/home/barti/Documents/IMAC/C/IMAGIMP/images/Clown.256.ppm");

	Layer* l1 = createLayer(test, NULL, 0.5, SUM);
	Layer* l2 = createLayer(test, l1, 0.1, MULTIPLICATION);
	Layer* l3 = createLayer(test, l2, 1.0, SUM);
	Layer* l4 = createLayer(test, l3, 0.8, SUM);

	LayerList* ll1 = NULL;
	LayerList* ll2 = NULL;
	testCreate(&ll1,&ll2,l1);
	testGoCurrentNextPreviousHeadBottom(ll1,l1,l2,l3,l4);

}

void testCreate(LayerList** ll1, LayerList** ll2, Layer* l1){
	//Test création
	*ll1 = createLayerList(l1);
	*ll2 = createEmptyLayerList();
	
	dumpLayerList(*ll1);
	dumpLayerList(*ll2);
}

void testGoCurrentNextPreviousHeadBottom(LayerList* ll1, Layer* l1, Layer* l2, Layer* l3, Layer* l4){
	
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
