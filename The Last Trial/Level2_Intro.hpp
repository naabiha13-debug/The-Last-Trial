#ifndef LEVEL2_INTRO_HPP
#define LEVEL2_INTRO_HPP

#include "iGraphics.h"
#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"

bool level2Intro = false;
DWORD level2IntroStartTime = 0;
const DWORD LEVEL2_INTRO_DURATION = 3000; // 3 seconds
bool level2ShowingRules = false;
void drawLevel2Rules()
{
	iShowImage(0, 0, 1000, 600, level2Rules);

	iShowImage(800, 500, 140, 50, nextBtnImg);
}

void startLevel2Intro()
{
	level2Intro = true;
	level2IntroStartTime = GetTickCount();
}

void updateLevel2Intro()
{
	if (level2Intro)
	{
		if (GetTickCount() - level2IntroStartTime >= LEVEL2_INTRO_DURATION)
		{
			level2Intro = false;
		}
	}
}

void drawLevel2Intro()
{
	if (level2Intro)
	{
		iShowImage(
			200,
			200,
			600,
			200,
			level2StartImg
			);
	}
}

#endif