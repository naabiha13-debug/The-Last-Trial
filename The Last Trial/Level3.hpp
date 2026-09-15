#ifndef LEVEL3_HPP
#define LEVEL3_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include "Level3_Player.hpp"
#include "WaitingRoom.hpp"

void drawLevel3()
{
	iShowImage(0, 0, 1000, 600, waitingRoom);
	if (waitingRoomStage != 4)
		drawLevel3Player();
	drawWaitingRoomStage();
}

#endif