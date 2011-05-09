//These are the function implementations for the AreaRecognition class.


#include "StdAfx.h"
#include "AreaRecognition.h"

//The class counstructor for area recornition takes an image and using that image's
//width and height creates boundary values for a 4 x and y subdivision and for a 3 x and y subdivision of that image.
//All boundary variables are saved in member variables fo the class
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


//This function takes a point and using the 3 area y axis subdivision and 4 area x axis subdivision
//returns a point with the x and y areas in which the original point parameter is located
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


//This function takes a point and using the 3 area x axis subdivision and 4 area y axis subdivision
//returns a point with the x and y areas in which the original point parameter is located
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


//This function takes a point and using the 4 area y axis subdivision and 4 area x axis subdivision
//returns a point with the x and y areas in which the original point parameter is located
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


//This function takes a point and using the 4 area x axis subdivision
//returns an int with the x area in which the original point parameter is located
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

////This function takes a point and using the 3 area x axis subdivision
//returns an int with the x area in which the original point parameter is located
int AreaRecognition::AreaWideX(CvPoint location){
	
	if (location.x == 0 && location.y == 0)
		return 0;
	
	if(location.x < wideLeftX)
		return 1;
	else if (location.x > wideRightX)
		return 3;
	else return 2;
}


////This function takes a point and using the 3 area y axis subdivision
//returns an int with the y area in which the original point parameter is located
int AreaRecognition::AreaWideY(CvPoint location){
	
	if (location.x == 0 && location.y == 0)
		return 0;
	
	if(location.y < topY)
		return 1;
	else if (location.y > bottomY)
		return 3;
	else return 2;
}