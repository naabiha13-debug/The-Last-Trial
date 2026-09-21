#ifndef LEVEL3_HPP
#define LEVEL3_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include "Level3_Player.hpp"
#include "WaitingRoom.hpp"
#include "Jungle.hpp"
#include "river.hpp"

void drawLevel3()
{
	if (waitingRoomStage == 6)
	{
		drawJungleBackground();
		drawJungleProps();
		drawJungleRocks();
		drawJungleDolls();
		drawJunglePapers();
		drawJunglePlayer();
		drawDontMoveSign();
		drawDoorTransition();
		return;
	}

	iShowImage(0, 0, 1000, 600, waitingRoom);
	if (waitingRoomStage != 4)
		drawLevel3Player();
	drawWaitingRoomStage();
	drawDoorTransition();
}

#endif