#ifndef LEVEL3_JUNGLE_HPP
#define LEVEL3_JUNGLE_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"

// Camera/world scroll offset
int jungleWorldX = 0;
const int jungleTotalWidth = 3000;
const int jungleScreenWidth = 1000;
const int jungleMaxScroll = jungleTotalWidth - jungleScreenWidth; // 2000



int jungleFrame = 0;
int jungleWalkTimer = 0;
bool jungleFacingRight = true;

// 3 dolls, one per bg segment, static world position
int dollWorldX[3] = { 500, 1500, 2500 };
int dollY = 180;

int dollFrame[3] = { 4, 4, 4 };   // start idle (d5)
int dollState[3] = { 0, 0, 0 };   // 0=idle,1=turning to face,2=holding,3=turning back
int dollTimer[3] = { 0, 0, 0 };
int dollCyclesDone[3] = { 0, 0, 0 };
const int dollMaxCycles = 3;
int playerWorldX = 100;              // player's actual position in world
const int jungleMiddleX = 465;       
int jungleScreenPlayerY = 110;
bool jungleDead = false;

int paperWorldX[3] = { 650, 1650, 2650 };
int paperY = 100;
bool paperCollected[3] = { false, false, false };

bool pickingUpPaper = false;
int pickupPaperFrame = 0;
int pickupPaperTimer = 0;
int activePaperIndex = -1;


void drawJungleBackground()
{
	for (int i = 0; i < 3; i++)
	{
		int bgScreenX = (i * jungleScreenWidth) - jungleWorldX;
		iShowImage(bgScreenX, 0, jungleScreenWidth, 600, jungleBgImg[i]);
	}
}

void drawJungleDolls()
{
	for (int i = 0; i < 3; i++)
	{
		int screenX = dollWorldX[i] - jungleWorldX;
		if (screenX > -100 && screenX < 1100)
			iShowImage(screenX, dollY, 80, 150, dollImg[dollFrame[i]]);
	}
}

void drawJunglePlayer()
{
	int screenPlayerX = playerWorldX - jungleWorldX;

	if (jungleDead)
	{
		return;
	}

	if (pickingUpPaper)
	{
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, pickupImg[pickupPaperFrame]);
		return;
	}

	if (jungleFacingRight)
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, l3walkFImg[jungleFrame]);
	else
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, l3walkBImg[jungleFrame]);
}

void updateJungleDolls()
{
	for (int i = 0; i < 3; i++)
	{

		dollTimer[i]++;

		if (dollState[i] == 0)
		{
			if (dollTimer[i] >= 90)
			{
				dollTimer[i] = 0;
				dollState[i] = 1;
			}
		}
		else if (dollState[i] == 1)
		{
			if (dollTimer[i] >= 20)
			{
				dollTimer[i] = 0;
				dollFrame[i]--;
				if (dollFrame[i] <= 0)
				{
					dollFrame[i] = 0;
					dollState[i] = 2;
				}
			}
		}
		else if (dollState[i] == 2)
		{
			if (dollTimer[i] >= 80)
			{
				dollTimer[i] = 0;
				dollState[i] = 3;
			}
		}
		else if (dollState[i] == 3)
		{
			if (dollTimer[i] >= 20)
			{
				dollTimer[i] = 0;
				dollFrame[i]++;
				if (dollFrame[i] >= 4)
				{
					dollFrame[i] = 4;
					dollState[i] = 0;
					dollCyclesDone[i]++;
				}
			}
		}
	}
}
void drawJunglePapers()
{
	for (int i = 0; i < 3; i++)
	{
		if (!paperCollected[i] && i != activePaperIndex)
		{
			int screenX = paperWorldX[i] - jungleWorldX;
			if (screenX > -50 && screenX < 1050)
				iShowImage(screenX, paperY, 80, 80, paperImg);
		}
	}
}

void updateJungle()
{
		if (jungleDead)
			return;

		if (pickingUpPaper)
		{
			pickupPaperTimer++;
			if (pickupPaperTimer >= 8)
			{
				pickupPaperTimer = 0;
				pickupPaperFrame++;
				if (pickupPaperFrame > 4)
				{
					pickingUpPaper = false;
					pickupPaperFrame = 0;
					if (activePaperIndex != -1)
						paperCollected[activePaperIndex] = true;
					activePaperIndex = -1;
				}
			}
			return;
		}

		// Down arrow: kache paper thakle pickup shuru
		if (isSpecialKeyPressed(GLUT_KEY_DOWN))
		{
			for (int i = 0; i < 3; i++)
			{
				if (!paperCollected[i] && abs(playerWorldX - paperWorldX[i]) < 60)
				{
					pickingUpPaper = true;
					pickupPaperFrame = 0;
					pickupPaperTimer = 0;
					activePaperIndex = i;
					return;
				}
			}
		}

	

		bool isMoving = isSpecialKeyPressed(GLUT_KEY_RIGHT) || isSpecialKeyPressed(GLUT_KEY_LEFT);

		if (isMoving)
		{
			for (int i = 0; i < 3; i++)
			{
				if (dollState[i] == 2)  
				{
					jungleDead = true;
					return;
				}
			}
		}

	
	const int playerWidth = 120;
	int worldMax = jungleTotalWidth - playerWidth;   

	if (isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		jungleFacingRight = true;

		if (playerWorldX < worldMax)
			playerWorldX += 5;

	
		if ((playerWorldX - jungleWorldX) > jungleMiddleX && jungleWorldX < jungleMaxScroll)
			jungleWorldX += 5;

		if (jungleWorldX > jungleMaxScroll)
			jungleWorldX = jungleMaxScroll;

		jungleWalkTimer++;
		if (jungleWalkTimer >= 5)
		{
			jungleWalkTimer = 0;
			jungleFrame++;
			if (jungleFrame > 8)
				jungleFrame = 0;
		}
	}
	else if (isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		jungleFacingRight = false;

		if (playerWorldX > 0)
			playerWorldX -= 5;

		if ((playerWorldX - jungleWorldX) < jungleMiddleX && jungleWorldX > 0)
			jungleWorldX -= 5;

		if (jungleWorldX < 0)
			jungleWorldX = 0;

		jungleWalkTimer++;
		if (jungleWalkTimer >= 5)
		{
			jungleWalkTimer = 0;
			jungleFrame++;
			if (jungleFrame > 8)
				jungleFrame = 0;
		}
	}
	else
	{
		jungleWalkTimer = 0;
		jungleFrame = 0;
	}
}
void drawDontMoveSign()
{
	for (int i = 0; i < 3; i++)
	{
		if (dollState[i] == 1 || dollState[i] == 2)
		{
			iShowImage(400, 500, 200, 80, dontMoveImg); 
			return;  
		}
	}
}

#endif