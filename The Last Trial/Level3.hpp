#ifndef LEVEL3_HPP
#define LEVEL3_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include "Level3_Player.hpp"
#include "WaitingRoom.hpp"
#include "Jungle.hpp"
#include "river.hpp"
#include "Pinkeypad addition.hpp"

#include "ControlRoom.hpp"  

// Position of the buttons under the game-over image — change these to move them.
const int gameOverAgainX = 400, gameOverAgainY = 140, gameOverAgainW = 200, gameOverAgainH = 60;
const int gameOverBackX = 400, gameOverBackY = 60, gameOverBackW = 200, gameOverBackH = 60;

bool isGameOver()
{
	return jungleDead || pinWrong || crPlayerDefeated;
}

// Clears the specific death flag and resets only what's needed so the player
// resumes right where they died, as if they never lost.
void retryFromGameOver()
{
	if (jungleDead)
	{
		jungleDead = false;
		for (int i = 0; i < 3; i++)
		{
			dollState[i] = 0;
			dollFrame[i] = 4;
			dollTimer[i] = 0;
		}
	}
	else if (pinWrong)
	{
		resetPinEntry();
		showPinFinal = true;   // keypad shows again so they can retry
	}
	else if (crPlayerDefeated)
	{
		crPlayerDefeated = false;
		crCombatOver = false;
		crGuardHitsOnPlayer = 0;
		crPlayerHitsOnGuard = 0;
		crMaskRawHitsOnPlayer = 0;
		crPlayerRawHitsOnMask = 0;
	}
	playLevel3Music();
}

void drawGameOverScreen()
{
	if (jungleDead)
		iShowImage(0, 0, 1000, 600, gameOverJungleImg);
	else if (pinWrong)
		iShowImage(0, 0, 1000, 600, gameOverRiverImg);
	else if (crPlayerDefeated)
		iShowImage(0, 0, 1000, 600, gameOverControlImg);

	iShowImage(gameOverAgainX, gameOverAgainY, gameOverAgainW, gameOverAgainH, againBtnImg);
	iShowImage(gameOverBackX, gameOverBackY, gameOverBackW, gameOverBackH, backBtnImg);
}

void drawLevel3()
{
	if (isGameOver())
	{
		drawGameOverScreen();
		return;
	}

	if (waitingRoomStage == 6)
	{
		if (showLevel3Gp1)
		{
			iShowImage(0, 0, 1000, 600, level3gp1Img);
			iShowImage(gpNextBtnX, gpNextBtnY, gpNextBtnW, gpNextBtnH, nextImg);
			return;
		}

		if (pinUnlocked && !showPinFinal)
		{
			if (showLevel3Gp2)
			{
				iShowImage(0, 0, 1000, 600, level3gp2Img);
				iShowImage(gpNextBtnX, gpNextBtnY, gpNextBtnW, gpNextBtnH, nextImg);
				return;
			}

			drawControlRoom();
			return;
		}

		drawJungleBackground();
		drawJungleRocks();
		drawJungleDolls();
		drawJunglePapers();
		drawJunglePlayer();

		drawMsg3();
		drawMsg4();
		drawPinFinal();
		drawPinDisplay();
		drawMImage();
		drawMergeImage();
		drawDontMoveSign();
		drawDoorTransition();
		drawDistanceCounter();
		drawHintCounter();
		return;
	}

	iShowImage(0, 0, 1000, 600, waitingRoom);
	if (waitingRoomStage != 4)
		drawLevel3Player();
	drawWaitingRoomStage();
	drawDoorTransition();
}

#endif