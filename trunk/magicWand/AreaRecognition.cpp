#include "StdAfx.h"
#include "AreaRecognition.h"


AreaRecognition::AreaRecognition(IplImage* image)
{
	int narrowX = image->width / 4;
	int narrowY = image->height / 4;
	int wideX = image ->width / 3;
	int wideY = image ->height /3;

	leftX = narrowX;
	centerX = narrowX * 2;
	rightX = narrowX * 3;
	topY = narrowY;
	middleY = narrowY * 2;
	bottomY = narrowY * 3;
	wideTopY = wideY;
	wideBottomY = wideY * 2;
	wideLeftX = wideX;
	wideRightX = wideX * 2;
}


AreaRecognition::~AreaRecognition(void)
{
}

CvPoint AreaRecognition::AreasNarrowX(CvPoint location){
	int areaX;
	int areaY;

	if (location.x == 0 && location.y == 0)
		return cvPoint(0, 0);
	else{
		if (location.x <= leftX)
			areaX = 1;
		else if (location.x > rightX)
			areaX = 4;
		else if (location.x > leftX && location.x <= centerX)
			areaX =  2;
		else 
			areaX = 3;

		if(location.y < wideTopY)
			areaY = 1;
		else if (location.y > wideBottomY)
			areaY =  3;
		else areaY =  2;

		return cvPoint(areaX, areaY);
	}

}

CvPoint AreaRecognition::AreasNarrowY(CvPoint location){
	int areaX;
	int areaY;

	if (location.x == 0 && location.y == 0)
		return cvPoint(0, 0);


	if (location.y <= topY)
		areaY = 1;
	else if (location.y > bottomY)
		areaY = 4;
	else if (location.y > topY && location.y <= middleY)
		areaY =  2;
	else 
		areaY = 3;

	if(location.x < wideLeftX)
		areaX = 1;
	else if (location.x > wideRightX)
		areaX =  3;
	else areaX =  2;

	return cvPoint(areaX, areaY);

}

CvPoint AreaRecognition::AreasNarrowAll(CvPoint location){
	int areaX;
	int areaY;

	if (location.x == 0 && location.y == 0)
		return cvPoint(0, 0);

	if (location.x <= leftX)
		areaX = 1;
	else if (location.x > rightX)
		areaX = 4;
	else if (location.x > leftX && location.x <= centerX)
		areaX =  2;
	else 
		areaX = 3;

	if (location.y <= topY)
		areaY = 1;
	else if (location.y > bottomY)
		areaY = 4;
	else if (location.y > topY && location.y <= middleY)
		areaY =  2;
	else 
		areaY = 3;

	return cvPoint(areaX, areaY);

}


int AreaRecognition::AreaNarrowX(CvPoint location){
	if (location.x == 0 && location.y == 0)
		return 0;
	
	if (location.x <= leftX)
		return 1;
	else if (location.x > rightX)
		return 4;
	else if (location.x > leftX && location.x <= centerX)
		return 2;
	else 
		return 3;
}

int AreaRecognition::AreaWideX(CvPoint location){
	
	if (location.x == 0 && location.y == 0)
		return 0;
	
	if(location.x < wideLeftX)
		return 1;
	else if (location.x > wideRightX)
		return 3;
	else return 2;
}

int AreaRecognition::AreaWideY(CvPoint location){
	
	if (location.x == 0 && location.y == 0)
		return 0;
	
	if(location.y < topY)
		return 1;
	else if (location.y > bottomY)
		return 3;
	else return 2;
}