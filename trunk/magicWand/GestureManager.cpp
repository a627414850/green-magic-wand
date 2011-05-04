#include "StdAfx.h"
#include "GestureManager.h"


GestureManager::GestureManager(int historySizeX, int historySizeY, int historySizeCircle)
{
	historyX = historySizeX;
	historyY = historySizeY;
	historyCircle = historySizeCircle;
	areasNarrowX.push_back(cvPoint(0, 0));
	narrowXStartTimes.push_back(time(NULL));
	narrowXEndTimes.push_back(time(NULL));

	areasNarrowY.push_back(cvPoint(0, 0));
	narrowYStartTimes.push_back(time(NULL));
	narrowYEndTimes.push_back(time(NULL));

	areasCircle.push_back(cvPoint(0, 0));
	circleStartTimes.push_back(time(NULL));
	circleEndTimes.push_back(time(NULL));
}


GestureManager::~GestureManager(void)
{
}

void GestureManager::NextAreasNX(CvPoint currentAreas){
	if (currentAreas.x == 0 && currentAreas.y == 0);
	else if (currentAreas.x == areasNarrowX.back().x && currentAreas.y == areasNarrowX.back().y){
		narrowXEndTimes.pop_back();
		narrowXEndTimes.push_back(time(NULL));
	}
	else{

		areasNarrowX.push_back(currentAreas);
		narrowXStartTimes.push_back(time(NULL));
		narrowXEndTimes.push_back(time(NULL));

		if (areasNarrowX.size() > historyX){
			narrowXEndTimes.pop_front();
			narrowXStartTimes.pop_front();
			areasNarrowX.pop_front();
		}
		cout << "X Focus Areas start!! \n";
		cout <<"Time between first and last: " << difftime(narrowXStartTimes.back(), narrowXEndTimes.front()) << "\n";
		for(int i = 0; i < areasNarrowX.size(); i++)
			cout<< "AreaX: " << areasNarrowX[i].x << "  AreaY: " << areasNarrowX[i].y << "  Time Start: " << narrowXStartTimes[i] << "Time End: " << narrowXEndTimes[i] << "\n";
	}
}

void GestureManager::NextAreasNY(CvPoint currentAreas){
	if (currentAreas.x == 0 && currentAreas.y == 0);
	else if (currentAreas.x == areasNarrowY.back().x && currentAreas.y == areasNarrowY.back().y){
		narrowYEndTimes.pop_back();
		narrowYEndTimes.push_back(time(NULL));
	}
		
	else{

		areasNarrowY.push_back(currentAreas);
		narrowYStartTimes.push_back(time(NULL));
		narrowYEndTimes.push_back(time(NULL));

		if (areasNarrowY.size() > historyY){
			narrowYEndTimes.pop_front();
			narrowYStartTimes.pop_front();
			areasNarrowY.pop_front();
		}
		cout << "Y Focus Areas start!! \n";
		cout <<"Time between first and last: " << difftime(narrowYStartTimes.back(), narrowYEndTimes.front()) << "\n";
		for(int i = 0; i < areasNarrowY.size(); i++)
			cout<< "AreaX: " << areasNarrowY[i].x << "  AreaY: " << areasNarrowY[i].y << "  Time: " "  Time Start: " << narrowYStartTimes[i] << "Time End: " << narrowYEndTimes[i] << "\n";
	}
}

void GestureManager::NextAreasCircle(CvPoint currentAreas){
	if (currentAreas.x == 0 && currentAreas.y == 0);
	else if (currentAreas.x == areasCircle.back().x && currentAreas.y == areasCircle.back().y){
		circleEndTimes.pop_back();
		circleEndTimes.push_back(time(NULL));
	}
		
	else{

		areasCircle.push_back(currentAreas);
		circleStartTimes.push_back(time(NULL));
		circleEndTimes.push_back(time(NULL));

		if (areasCircle.size() > historyCircle){
			areasCircle.pop_front();
			circleStartTimes.pop_front();
			circleEndTimes.pop_front();
		}
		cout << "Circle Areas start!! \n";
		cout <<"Time between first and last: " << difftime(circleStartTimes.back(), circleEndTimes.front()) << "\n";
		for(int i = 0; i < areasCircle.size(); i++)
			cout<< "AreaX: " << areasCircle[i].x << "  AreaY: " << areasCircle[i].y << "  Time: " "  Time Start: " << circleStartTimes[i] << "Time End: " << circleEndTimes[i] << "\n";
	}
}

bool GestureManager::HorizontalGesture(){
	
	if(areasNarrowX.front().x == 1 && difftime(narrowXEndTimes.back(), narrowXEndTimes.front()) < 3 && areasNarrowX.size() == historyX){
		int previousX = 1;
		int firstY = areasNarrowX.front().y;
		for (int i = 1; i < areasNarrowX.size(); i++){
			if((previousX + 1) == areasNarrowX[i].x && firstY == areasNarrowX[i].y)
				previousX = areasNarrowX[i].x;
			else return false;
		}
		cout << "Horizontal Gesture!! \n";
		return true;
	}
	else return false;
}

bool GestureManager::VerticalGesture(){
	if(areasNarrowY.front().y == 1 && difftime(narrowYEndTimes.back(), narrowYEndTimes.front()) < 3 && areasNarrowY.size() == historyY){
		int previousY = 1;
		int firstX = areasNarrowY.front().x;
		for (int i = 1; i < areasNarrowY.size(); i++){
			if((previousY + 1) == areasNarrowY[i].y && firstX == areasNarrowY[i].x)
				previousY = areasNarrowY[i].y;
			else return false;
		}
		cout << "Vertical Gesture!! \n";
		return true;
	}
	else return false;
}

bool GestureManager::CircularGesture(){
	if(difftime(circleEndTimes.back(), circleEndTimes.front()) < 4 && areasCircle.size() == historyCircle){
		if(areasCircle.front().x + 1 == areasCircle[1].x && areasCircle.front().y == areasCircle[1].y && 
			areasCircle.back().x == areasCircle.front().x && areasCircle.back().y == areasCircle.front().y &&
			areasCircle[3].x + 1 == areasCircle[2].x && areasCircle[3].y == areasCircle[2].y && areasCircle[1].y + 1 == areasCircle[2].y){
			cout << "Circle Gesture!!************************************************************************ \n";
			return true;
		}
		else if(areasCircle.front().y + 1 == areasCircle[1].y && areasCircle.front().x == areasCircle[1].x &&
			areasCircle.back().x == areasCircle.front().x && areasCircle.back().y == areasCircle.front().y &&
			areasCircle[3].y + 1 == areasCircle[2].y && areasCircle[3].x == areasCircle[2].x && areasCircle[2].x + 1 == areasCircle[1].x){
			cout << "Circle Gesture!!************************************************************************ \n";
			return true;
		}
		else if(areasCircle.front().x - 1 == areasCircle[1].x && areasCircle.front().y == areasCircle[1].y && 
			areasCircle.back().x == areasCircle.front().x && areasCircle.back().y == areasCircle.front().y &&
			areasCircle[3].x - 1 == areasCircle[2].x && areasCircle[3].y == areasCircle[2].y && areasCircle[1].y - 1 == areasCircle[2].y){
			cout << "Circle Gesture!!************************************************************************ \n";
			return true;
		}
		else if(areasCircle.front().y - 1 == areasCircle[1].y && areasCircle.front().x == areasCircle[1].x &&
			areasCircle.back().x == areasCircle.front().x && areasCircle.back().y == areasCircle.front().y &&
			areasCircle[3].y - 1 == areasCircle[2].y && areasCircle[3].x == areasCircle[2].x && areasCircle[2].x - 1 == areasCircle[1].x){
			cout << "Circle Gesture!!************************************************************************ \n";
			return true;
		}
		else return false;
	}
	else return false;
}

bool GestureManager::PointGesture(){
	if (difftime(narrowYEndTimes.back(), narrowYStartTimes.back()) >= 3){
		cout << "POINT GESTURE!! \n";
		return true;
	}
	else
		return false;
}

bool GestureManager::FlickGesture(){

	if (narrowYEndTimes.size() == historyY && (difftime(narrowYEndTimes.back(), narrowYEndTimes[narrowYEndTimes.size() - 3]) < 2) && ( areasNarrowY[areasNarrowY.size() - 3].x == areasNarrowY.back().x) &&
		(areasNarrowY[areasNarrowY.size() - 3].y == areasNarrowY.back().y) && (abs((areasNarrowY[areasNarrowY.size() - 2].y - areasNarrowY[areasNarrowY.size() - 3].y)) == 1)
		&& (areasNarrowY[areasNarrowY.size() - 2].x == areasNarrowY[areasNarrowY.size() - 3].x)){
			cout << "FLICK GESTURE!!!!! \n";
			return true;
	}
	else return false;
}

