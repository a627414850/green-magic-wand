#pragma once


#include <iostream>
#include <string>
#include "AreaRecognition.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


enum Spell {noSpell, greenSpell, purpleSpell, orangeSpell};

class GameScreen
{

	
public:
	IplImage* currentMonster;
	IplImage* orangeMonster;
	IplImage* purpleMonster;
	IplImage* greenMonster;
	IplImage* wand;
	Spell killSpell;
	int monsterArea;
	CvPoint areaCoordinate;



	GameScreen(void);
	~GameScreen(void);

	void GameScreen::CreateScene();
	IplImage* GameScreen::showScene(CvPoint currentPoint, Spell currentSpell);
};

