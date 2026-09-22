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

void drawLevel3()
{
	if (waitingRoomStage == 6)
	{
		if (pinUnlocked)
		{
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