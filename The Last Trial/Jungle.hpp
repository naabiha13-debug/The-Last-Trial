#ifndef LEVEL3_JUNGLE_HPP
#define LEVEL3_JUNGLE_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include <math.h>
#include "river.hpp"

// Camera/world scroll offset
int jungleWorldX = 0;
const int jungleTotalWidth = 7000;
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

int rockWorldX[3];
const int rockWidth = 120;
const int rockHeight = 100;
const int rockY = 100; // ground-aligned with player feet (jungleScreenPlayerY 110 + height 150 = 260)

void initJungleRocks()
{
	for (int i = 0; i < 3; i++)
	{
		int screenStart = i * jungleScreenWidth;
		rockWorldX[i] = screenStart + 300 + (rand() % 400); // random within each screen, away from edges
	}
}

bool jumpingJungle = false;
int jungleJumpFrame = 0;
int jungleJumpTimer = 0;

const int jumpMaxHeight = 140;
// jumpFrameCount is defined in Level3_Assets.hpp (8 frames)

void drawJungleBackground()
{
	for (int i = 0; i < 7; i++)
	{
		int bgScreenX = (i * jungleScreenWidth) - jungleWorldX;

		if (i < 3)
			iShowImage(bgScreenX, 0, jungleScreenWidth, 600, jungleBgImg[i]);
		else
			iShowImage(bgScreenX, 0, jungleScreenWidth, 600, riverBgImg[i - 3]);
	}

	drawRiverProps(jungleWorldX, 3);
}


void drawJungleProps()
{
	int propWidth = 210;   //  prop1.png actual width
	int propHeight = 210;  // actual height
	int boundaryWorldX = jungleScreenWidth; // = 1000, screen1/screen2  seam

	int propScreenX = boundaryWorldX - jungleWorldX - (propWidth / 2) - 5;
	iShowImage(propScreenX, 147, propWidth, propHeight, prop1Img);
}

void drawJungleRocks()
{
	for (int i = 0; i < 3; i++)
	{
		int screenX = rockWorldX[i] - jungleWorldX;
		iShowImage(screenX, rockY, rockWidth, rockHeight, rockImg);
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
	if (jumpingJungle)
	{
		// CHANGED: 5.0f -> (jumpFrameCount - 1), so 8 frames work
		float jumpProgress = jungleJumpFrame / (float)(jumpFrameCount - 1);
		int jumpOffset = (int)(sinf(jumpProgress * 3.14159f) * jumpMaxHeight);
		iShowImage(screenPlayerX, jungleScreenPlayerY + jumpOffset, 120, 150, jumpImg[jungleJumpFrame]);
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

	if (!jumpingJungle && isSpecialKeyPressed(GLUT_KEY_UP) && isSpecialKeyPressed(GLUT_KEY_RIGHT))
		jumpingJungle = true;

	if (jumpingJungle)
	{
		jungleJumpTimer++;
		if (jungleJumpTimer >= 4)
		{
			jungleJumpTimer = 0;
			jungleJumpFrame++;
			// CHANGED: > 5 -> >= jumpFrameCount, so all 8 frames play
			if (jungleJumpFrame >= jumpFrameCount)
			{
				jumpingJungle = false;
				jungleJumpFrame = 0;

				for (int i = 0; i < 3; i++)
				{
					if (playerWorldX + 120 > rockWorldX[i] && playerWorldX < rockWorldX[i] + rockWidth)
						playerWorldX = rockWorldX[i] + rockWidth;
				}
			}
		}
	}
	bool isMoving = isSpecialKeyPressed(GLUT_KEY_RIGHT) || isSpecialKeyPressed(GLUT_KEY_LEFT);

	if (isMoving && playerWorldX < jungleScreenWidth * 3)
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

		bool blockedByRock = false;
		if (!jumpingJungle)
		{
			for (int i = 0; i < 3; i++)
			{
				if (playerWorldX + 120 > rockWorldX[i] && playerWorldX < rockWorldX[i] + rockWidth)
					blockedByRock = true;
			}
		}

		int moveSpeed = jumpingJungle ? 10 : 5;

		if (playerWorldX < worldMax && !blockedByRock)
			playerWorldX += moveSpeed;

		if ((playerWorldX - jungleWorldX) > jungleMiddleX && jungleWorldX < jungleMaxScroll)
			jungleWorldX += moveSpeed;

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
	if (playerWorldX >= jungleScreenWidth * 3)
		return;

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