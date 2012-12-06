#include "testListes.h"
#include "layer.h"
#include "list.h"
#include "image.h"
#include <stdlib.h>

//Variables globales de test
Image* test= NULL ;
Layer* l1 = NULL, *l2= NULL, *l3= NULL, *l4= NULL, *lInsert1= NULL, *lInsert2= NULL,*lInsert3= NULL, *lInsert4= NULL, *lInsert5= NULL, *lInsert6= NULL, *lInsert7= NULL, *lInsert8 = NULL;
List* ll1= NULL, *ll2= NULL;
Cell *c1= NULL,*c2= NULL,*c3= NULL,*c4= NULL,*c5= NULL,*c6= NULL,*c7= NULL,*c8= NULL,*c9= NULL,*c10= NULL,*c11= NULL,*c12= NULL;
int main(int argc, char** argv){
	if(argc == 2){
		test = loadImage(argv[1]);
	}
	else{
		test = loadImage("../images/Clown.256.ppm");
	}
	if(test==NULL)return -1;

	l1 = createLayer(test, 0.5, SUM);
	l2 = createLayer(test, 0.1, MULTIPLICATION);
	l3 = createLayer(test, 1.0, SUM);
	l4 = createLayer(test, 0.8, SUM);
	
	//Ceux là ne seront pas connectés mais rajoutés à la main dans la liste
	lInsert1 = createLayer(test, 0.8, SUM);
	lInsert2 = createLayer(test, 0.8, SUM);
	lInsert3 = createLayer(test, 0.4, SUM);
	lInsert4 = createLayer(test, 0.8, SUM);
	lInsert5 = createLayer(test, 0.8, SUM);
	lInsert6 = createLayer(test, 0.8, SUM);
	lInsert7 = createLayer(test, 0.8, SUM);
	lInsert8 = createLayer(test, 0.8, SUM);
	
	
	c2  = createCell(l2);
	c3  = createCell(l3);
	c4  = createCell(l4);
	
	/*Ancienne version des listes,
	la nouvelle ne demande pas de créer manuellement
	les cellules.
	c5  = createCell(lInsert1);
	c6  = createCell(lInsert2);
	c7  = createCell(lInsert3);
	c8  = createCell(lInsert4);
	c9  = createCell(lInsert5);
	c10 = createCell(lInsert6);
	c11 = createCell(lInsert7);
	c12 = createCell(lInsert8);
	*/
	
	//On connecte à la main (puisque ce sont des tests sur les fonctions
	//on ne peut pas utiliser les fonctions d'insertion)
	//On connecte c1 et c2 lors de l'initialisation de la liste
	//car c1 n'existe pas avant (voir testCreate()).
	c2->next = c3;
	c3->previous = c2;
	c3->next = c4;
	c4->previous = c3;


	ll1 = NULL;
	ll2 = NULL;
	testCreate();
	testCountElem();
	testIsEmpty();
	testGoCurrentNextPreviousHeadBottom();
	testInsert();
	testCountElemAfterInsert();
	testIsEmpty();
	
	testPosition();
	
	return 0;

}

void testCreate(){
	//Test création
	ll1 = createList(LAYER, l1);
	//On connecte la suite de la liste
	c1 = ll1->head;
	c1->next = c2;
	c2->previous = c1;
	//Du coup on doit indiquer la queue à la main
	//et la taille de la liste à la main
	ll1->bottom = c4;
	ll1->size = 4;
	ll2 = createEmptyList(LAYER);
	
	dumpList(ll1);
	dumpList(ll2);
}

void testCountElem(){
	printf("Test ListCountElem on normal list.\n");
	if(listCountElem(ll1) != 4){
		printf("Test ListCountElem on normal list failed.\n");
		exit(-1);
	}
	printf("=>OK\n");
	
	printf("Test ListCountElem on empty list.\n");
	if(listCountElem(ll2) != 0){
		printf("Test ListCountElem on empty list failed.\n");
		exit(-1);
	}
	printf("=>OK\n");
}

void testIsEmpty(){
	printf("Test isListEmpty on an empty list.\n");
	if(!isListEmpty(ll2)){
		printf("Test isListEmpty on an empty list failed.\n");
		exit(-1);
	}
	printf("=>OK\n");
	printf("Test isListEmpty on a non empty list.\n");
	if(isListEmpty(ll1)){
		printf("Test isListEmpty on a non empty list failed.\n");
		exit(-1);
	}
	printf("=>OK\n");
}

void testCountElemAfterInsert(){
	printf("Test ListCountElem on normal list.\n");
	if(listCountElem(ll1) != 12){
		printf("Test ListCountElem on normal list failed.\n");
		exit(-1);
	}
	printf("=>OK\n");
	
	printf("Test ListCountElem on empty list.\n");
	if(listCountElem(ll2) != 0){
		printf("Test ListCountElem on empty list failed.\n");
		exit(-1);
	}
	printf("=>OK\n");
}

void testGoCurrentNextPreviousHeadBottom(){
	
	printf( "Test goToHead\n");
	goToHeadList(ll1);
	if(ll1->cursor != NULL){
		printf( "Test goToHead failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test previous from head\n");
	if(previousCell(ll1) != NULL){
		printf("Test previous from head failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test currentCell from head.\n");
	if(currentCell(ll1) != NULL){
		printf( "Test currentCell from head failed.\n");
		exit(-1);
	
	}

	printf("Test next from head.\n");
	if(nextCell(ll1) != c1){
		printf("Test next from head failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf("Test previous from first position.\n");
	if(previousCell(ll1) != NULL){
		printf("Test previous from first position failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	ll1->cursor = c1;
	
	printf( "Test normal currentCell.\n");
	if(currentCell(ll1) != c1){
		printf( "Test normal currentCell failed.\n");
		exit(-1);
	
	}
	printf( "=>OK\n");
	
	printf("Test normal next.\n");
	if(nextCell(ll1) != c2){
		printf("Test normal next failed.\n");
		exit(-1);		
	}
	printf( "=>OK\n");
	
	printf("Test normal previous.\n");
	if(previousCell(ll1) != c1){
		printf("Test normal previous failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	goToBottomCell(ll1);

	printf("Test goToBottom.\n");
	if(ll1->cursor != c4){
		printf("Test goToBottom failed. Cursor value = %p, c4 value = %p \n", ll1->cursor, c4);
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test currentCell from bottom.\n");
	if(currentCell(ll1) != c4){
		printf( "Test currentCell from bottom failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test next from bottom.\n");
	if(nextCell(ll1) != NULL){
		printf( "Test next from bottom failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test previous from bottom.\n");
	if(previousCell(ll1) != c3){
		printf( "Test previous from bottom failed.\n");
		exit(-1);
	}	
	printf( "=>OK\n");
}	
	
void testInsert(){
	printf("Test InsertHead\n");
	
	c5 = insertHeadCell(ll1, lInsert1);
	
	if(ll1->head != c5 || ll1->head->userData != lInsert1){
		printf( "Test insertHeadCell failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
	printf("Test insertbottom\n");
	
	c6 = insertBottomCell(ll1,lInsert2);
	
	if(ll1->bottom != c6 || c4->next != c6 || c6->previous != c4){
		printf( "Test insertbottom failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	goToBottomCell(ll1);
	
	printf( "Test insertAfterCell cursor on bottom\n");
	c7 = insertAfterCell(ll1, lInsert3);
	if(ll1->bottom != c7 || c6->next != c7 || c7->previous != c6 ){
		printf( "Test insertAfterCell cursor on bottom failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test insertAfterCell cursor normal\n");
	ll1->cursor = c2;
	
	c8 = insertAfterCell(ll1, lInsert4);
	if(nextCell(ll1) != c8 || c8->previous != c2 || c2->next != c8){
		printf( "Test insertAfterCell cursor normal failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test insertAfterCell cursor on head\n");
	goToHeadList(ll1);

	c9 = insertAfterCell(ll1, lInsert5);
	if(nextCell(ll1) != c9 || c9->next != c5 || c5->previous != c9 ){
		printf( "Test insertAfterCell cursor on head failed\n");
		exit(-1);
	}
	printf( "=>OK\n");

	printf( "Test insertBeforeCell cursor on head\n");
	
	goToHeadList(ll1);
	c10 = insertBeforeCell(ll1, lInsert6);
	
	if(nextCell(ll1) != c10 || c10->next != c9 || c9->previous != c10){
		printf("Test insertBeforeCell cursor on head failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test insertBeforeCell cursor normal \n");
	
	ll1->cursor = c3;
	c11 = insertBeforeCell(ll1, lInsert7);
	
	if(previousCell(ll1) != c11 || c11->next != c3 || c3->previous != c11){
		printf("Test insertBeforeCell cursor normal failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test insertBeforeCell cursor on bottom\n");
	
	goToBottomCell(ll1);
	c12 = insertBeforeCell(ll1, lInsert8);
	
	if(previousCell(ll1) != c12 || c12->next != ll1->bottom 
	|| ll1->bottom->previous != c12){
		printf("Test insertBeforeCell cursor on bottom failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
}

void testPosition(){
	
	printf( "Test getCellByPosition\n");
	
	if(getCellByPosition(ll1,1) != ll1->head){
		printf( "Test getCellByPosition (pos 1) failed.\n");
		exit(-1);	
	}
	
	if(getCellByPosition(ll1, 12) != goToBottomCell(ll1)){
		printf( "Test getCellByPosition (pos 12 (bottom)) failed.\n");
		exit(-1);
	}
	
	if(getCellByPosition(ll1, 4) != c1){
		printf( "Test getCellByPosition (pos 4 (random position)) failed.\n");
		exit(-1);
	}
	printf( "=>OK\n");
	
	printf( "Test delCellByPosition\n");
	int initSize = ll1->size;
	if(delCellByPosition(ll1, 1) != c10 || ll1->head == c10
	|| ll1->head != c9 || ll1->size != (initSize-1)){
		printf( "Test delCellByPosition (head) failed\n");
		exit(-1);
	}
	
	if(delCellByPosition(ll1, ll1->size) != c7 
	|| ll1->bottom == c7 || ll1->bottom != c12){
		printf( "Test delCellByPosition (bottom) failed\n");
		exit(-1);
	}
	
	if(delCellByPosition(ll1, ll1->size/2) != c8 
	|| c2->next != c11 || c11->previous != c2 ){
		printf( "Test delCellByPosition (random) failed\n");
		exit(-1);
	}
	printf( "=>OK\n");
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

