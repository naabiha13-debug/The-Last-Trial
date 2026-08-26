#ifndef LEVEL1_INTRO_HPP
#define LEVEL1_INTRO_HPP

#include "Level1_Character.hpp"

#define LEVEL1_PHASE_INTRO     0
#define LEVEL1_PHASE_COUNTDOWN 1
#define LEVEL1_PHASE_PLAYING   2

int level1Phase = LEVEL1_PHASE_INTRO;
int level1IntroPage = 0;

int level1IntroImg[3];
int introNextImg;
int introSkipImg;
int level1StartImg;

clock_t level1CountdownStart;

int introBtnW = 100;
int introBtnH = 44;

int introNextBtnX, introNextBtnY;
int introSkipBtnX, introSkipBtnY;

void loadLevel1IntroImages()
{
	static bool loaded = false;
	if (loaded) return;

	level1IntroImg[0] = iLoadImage("Image//Level1GamePlay1.png");
	level1IntroImg[1] = iLoadImage("Image//Level1GamePlay2.png");
	level1IntroImg[2] = iLoadImage("Image//Level1GamePlay3.png");

	introNextImg = iLoadImage("Image//next.png");
	introSkipImg = iLoadImage("Image//skip.png");

	level1StartImg = iLoadImage("Image//Level1Start.png");

	loaded = true;
}

void startLevel1Countdown()
{
	level1Phase = LEVEL1_PHASE_COUNTDOWN;
	level1CountdownStart = clock();
}

void enterLevel1()
{
	initLevel1();
	loadLevel1IntroImages();

	level1Phase = LEVEL1_PHASE_INTRO;
	level1IntroPage = 0;
}

void drawLevel1Intro()
{
	iShowImage(0, 0, LEVEL1_WIDTH, LEVEL1_HEIGHT, level1IntroImg[level1IntroPage]);

	int margin = 8;

	// NEXT
	introNextBtnX = LEVEL1_WIDTH - introBtnW - margin-35;
	introNextBtnY = LEVEL1_HEIGHT - introBtnH - margin-10;
	iShowImage(introNextBtnX, introNextBtnY, introBtnW, introBtnH, introNextImg);

	// SKIP
	if (level1IntroPage == 0)
	{
		introSkipBtnX = LEVEL1_WIDTH - introBtnW - margin-25;
		introSkipBtnY = margin+10;
		iShowImage(introSkipBtnX, introSkipBtnY, introBtnW, introBtnH, introSkipImg);
	}
}

void drawLevel1Countdown()
{
	iShowImage(0, 0, LEVEL1_WIDTH, LEVEL1_HEIGHT, backgroundImage);

	int dispW = 620;
	int dispH = (int)(dispW * 793.0 / 1983.0);

	int x = (LEVEL1_WIDTH - dispW) / 2;
	int y = (LEVEL1_HEIGHT - dispH) / 2;

	iShowImage(x, y, dispW, dispH, level1StartImg);
}

void updateLevel1Countdown()
{
	double elapsed =
		(double)(clock() - level1CountdownStart) / CLOCKS_PER_SEC;

	if (elapsed >= 3.0)
	{
		level1Phase = LEVEL1_PHASE_PLAYING;
		startLevel1();
	}
}

void level1IntroMouse(int button, int state, int mx, int my)
{
	if (button != GLUT_LEFT_BUTTON) return;
	if (state != GLUT_DOWN) return;

	bool hitNext =
		mx >= introNextBtnX && mx <= introNextBtnX + introBtnW &&
		my >= introNextBtnY && my <= introNextBtnY + introBtnH;

	if (hitNext)
	{
		if (level1IntroPage < 2)
		{
			level1IntroPage++;
		}
		else
		{
			startLevel1Countdown();
		}
		return;
	}

	if (level1IntroPage == 0)
	{
		bool hitSkip =
			mx >= introSkipBtnX && mx <= introSkipBtnX + introBtnW &&
			my >= introSkipBtnY && my <= introSkipBtnY + introBtnH;

		if (hitSkip)
		{
			startLevel1Countdown();
			return;
		}
	}
}

#endif