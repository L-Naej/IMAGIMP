#include "ihm.h"
#include "layersManager.h"
#include "list.h"
#include "utils.h"
#include <interface.h>

int main(int argc, char** argv){
	
	List* layerList = initLayersList(argc, argv);
	
	Image* secImage = loadImage("images/Clown.256.ppm");
	invertPPMArray(secImage->arrayRGB, secImage->width, secImage->height);
	Layer* testLayer = createLayer(secImage, 1.0, SUM);
	
	addLayer(layerList, testLayer);

	goToHeadList(layerList);
	
	Image* imgSource = ((Layer*)nextData(layerList))->imgSource;
	int width = imgSource->width;
	int height = imgSource->height;
	
	Image* finalImage = createEmptyImg(width, height);
	finalImage->maxValue = imgSource->maxValue;

	if(generateFinalImage(layerList, finalImage)){
		displayImage(finalImage);
	}
	
	return 0;
}
