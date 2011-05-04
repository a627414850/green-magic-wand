#pragma once


#include <iostream>
#include <string>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


class AreaRecognition
{
public:

	int leftX;
	int centerX;
	int rightX;
	int topY;
	int bottomY;
	int middleY;
	int wideTopY;
	int wideBottomY;
	int wideLeftX;
	int wideRightX;

	AreaRecognition(IplImage* image);
	~AreaRecognition(void);

	CvPoint AreasNarrowX(CvPoint location);
	CvPoint AreasNarrowY(CvPoint location);
	CvPoint AreasNarrowAll(CvPoint location);

	int AreaNarrowX(CvPoint location);
	int AreaWideX(CvPoint location);
	int AreaWideY(CvPoint location);
};

