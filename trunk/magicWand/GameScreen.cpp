#include "StdAfx.h"
#include "GameScreen.h"
#include <cmath>


GameScreen::GameScreen(void)
{

	IplImage* tempOrange = cvLoadImage("monster.png");
	IplImage* tempPurple = cvLoadImage("monster purple.jpg");
	IplImage* tempGreen = cvLoadImage("monster green.jpg");
	IplImage* tempWand = cvLoadImage("wand.jpg");
	orangeMonster = cvCreateImage(cvSize(tempOrange->width, tempOrange->height), 8, 3);
	purpleMonster = cvCreateImage(cvSize(tempPurple->width, tempPurple->height), 8, 3);
	greenMonster = cvCreateImage(cvSize(tempGreen->width, tempGreen->height), 8, 3);
	wand = cvCreateImage(cvSize(tempWand->width, tempWand->height), 8, 3);
	cvConvertImage(tempOrange, orangeMonster);
	cvConvertImage(tempPurple, purpleMonster);
	cvConvertImage(tempGreen, greenMonster);
	cvConvertImage(tempWand, wand);

	cvReleaseImage(&tempOrange);
	cvReleaseImage(&tempPurple);
	cvReleaseImage(&tempGreen);
	cvReleaseImage(&tempWand);

	CreateScene();


}


GameScreen::~GameScreen(void)
{
}

void GameScreen::CreateScene(){
	srand(time(NULL));
	monsterArea = rand() % 3 + 1;
	int newMonster = rand() % 3;

	if (monsterArea == 1)
		areaCoordinate =cvPoint(10,100);
	else if (monsterArea == 2)
		areaCoordinate =cvPoint(220,100);
	else if (monsterArea == 3)
		areaCoordinate =cvPoint(430,100);

	if (newMonster ==  0){
		currentMonster = orangeMonster;
		killSpell = orangeSpell;
	}
	else if(newMonster == 1){
		currentMonster = purpleMonster;
		killSpell = purpleSpell;
	}
	else if (newMonster == 2){
		currentMonster = greenMonster;
		killSpell = greenSpell;
	}

}

IplImage* GameScreen::showScene(CvPoint currentPoint, Spell currentSpell){
	IplImage*  workingImage= cvCreateImage(cvSize(640, 480), 8, 3);

	int maxY;
	int maxX;

	if (currentSpell == noSpell)
		cvRectangle(workingImage, cvPoint(0, 0), cvPoint(639, 479), CV_RGB(0, 0, 0), -1, 4);
	else if (currentSpell == greenSpell)
		cvRectangle(workingImage, cvPoint(0, 0), cvPoint(639, 479), CV_RGB(0, 250, 0), -1, 4);
	else if (currentSpell == purpleSpell)
		cvRectangle(workingImage, cvPoint(0, 0), cvPoint(639, 479), CV_RGB(255, 0, 255), -1, 4);
	else if(currentSpell == orangeSpell)
		cvRectangle(workingImage, cvPoint(0, 0), cvPoint(639, 479), CV_RGB(255, 100, 0), -1, 4);

	

	for (int y=areaCoordinate.y; y < areaCoordinate.y + currentMonster->height; y++ ) {
		uchar* ptr = (uchar*) (
			workingImage -> imageData + y * workingImage -> widthStep );
		uchar* ptrSmall = (uchar*) (
			currentMonster -> imageData + (y - areaCoordinate.y)  * currentMonster -> widthStep );

		for( int x=areaCoordinate.x;  x < areaCoordinate.x + currentMonster->width; x++ ) {
			if(ptrSmall[3 * (x - areaCoordinate.x)] > 220 &&  ptrSmall[3 * (x - areaCoordinate.x) + 1]> 220 && ptrSmall[3 * (x  - areaCoordinate.x) + 2] > 220);
			else{
				ptr[3 * x] = ptrSmall[3 * (x - areaCoordinate.x)];
				ptr[3 * x + 1] = ptrSmall[3 * (x - areaCoordinate.x) + 1];
				ptr[3 * x + 2] = ptrSmall[3 * (x  - areaCoordinate.x) + 2];
			}
		}
	}


	if(currentPoint.y + wand->height > workingImage -> height)
		maxY =  workingImage -> height;
	else
		maxY = currentPoint.y + wand->height;

	if(currentPoint.x + wand->width > workingImage -> width)
		maxX =  workingImage -> width;
	else
		maxX = currentPoint.x + wand->width;


	for (int y=currentPoint.y; y < maxY; y++ ) {
		uchar* ptr = (uchar*) (
			workingImage -> imageData + y * workingImage -> widthStep );
		uchar* ptrSmall = (uchar*) (
			wand -> imageData + (y - currentPoint.y)  * wand -> widthStep );

		for( int x=currentPoint.x; x < maxX; x++ ) {
			if(ptrSmall[3 * (x - currentPoint.x)] > 220 &&  ptrSmall[3 * (x - currentPoint.x) + 1]> 220 && ptrSmall[3 * (x  - currentPoint.x) + 2] > 220);
			else{
				ptr[3 * x] = ptrSmall[3 * (x - currentPoint.x)];
				ptr[3 * x + 1] = ptrSmall[3 * (x - currentPoint.x) + 1];
				ptr[3 * x + 2] = ptrSmall[3 * (x  - currentPoint.x) + 2];
			}
		}
	}

	return workingImage;
}
