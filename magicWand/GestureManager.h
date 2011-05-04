#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <time.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

enum Gestures{ horizontal, vertical, circle};


class GestureManager
{
public:

	deque<CvPoint> areasNarrowY;
	deque<time_t> narrowYStartTimes;
	deque<time_t> narrowYEndTimes;

	deque<CvPoint> areasNarrowX;
	deque<time_t> narrowXStartTimes;
	deque<time_t> narrowXEndTimes;

	deque<CvPoint> areasCircle;
	deque<time_t> circleStartTimes;
	deque<time_t> circleEndTimes;

	int historyX;
	int historyY;
	int historyCircle;

	GestureManager(int historySizeX, int historySizeY, int historySizeCircle);
	~GestureManager(void);

	void NextAreasNX(CvPoint currentAreas);
	void NextAreasNY(CvPoint currentAreas);
	void NextAreasCircle(CvPoint currentAreas);

	bool HorizontalGesture();
	bool VerticalGesture();
	bool CircularGesture();

	bool PointGesture();
	bool FlickGesture();
};

