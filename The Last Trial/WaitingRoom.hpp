#ifndef WAITINGROOM_HPP
#define WAITINGROOM_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include "Level3_Player.hpp"
#include "jungle.hpp"

// Cue object position 
int cueX = 930;
int cueY = 229;
int cueW = 56;
int cueH = 43;

// 0=idle, 1=msg1(over head), 2=msg2(fullscreen), 3=waiting for 'I',
// 4=take1->2->3 sequence, 5=warning1->2 sequence, 6=done
int waitingRoomStage = 0;

int wrTimer = 0;
int takeFrame = 0;
int warningFrame = 0;
bool iKeyWasPressed = false;

void drawWaitingRoomStage()
{
	iShowImage(cueX, cueY, cueW, cueH, cueImg);

	if (waitingRoomStage == 1)
		iShowImage(l3PlayerX - 60, l3PlayerY + 60, 250, 400, msg1Img);

	else if (waitingRoomStage == 2)
		iShowImage(0, 0, 1000, 600, msg2Img);

	else if (waitingRoomStage == 4)
	{
		iShowImage(l3PlayerX, l3PlayerY, 70, 90, pickupImg[takeFrame]);
	}
	else if (waitingRoomStage == 5)
	{
		if (warningFrame == 0) iShowImage(0, 0, 1000, 600, warning1Img);
		else if (warningFrame == 1) iShowImage(0, 0, 1000, 600, warning2Img);

		iShowImage(820, 520, 90, 50, nextImg);
	}
}

void updateWaitingRoomStage()
{
	if (waitingRoomStage == 4)
	{
		wrTimer++;
		if (wrTimer >= 20)   
		{
			wrTimer = 0;
			takeFrame++;
			if (takeFrame > 4)
			{
				takeFrame = 0;
				waitingRoomStage = 5;
			}
		}
	}


	if (waitingRoomStage == 3)
	{
		bool iPressed = isKeyPressed('i') || isKeyPressed('I');
		if (iPressed && !iKeyWasPressed)
		{
			waitingRoomStage = 4;
			takeFrame = 0;
			wrTimer = 0;
		}
		iKeyWasPressed = iPressed;
	}
}

void handleWaitingRoomClick(int mx, int my)
{
	// Stage 5: warning1/warning2 -> "Next" button click 
	if (waitingRoomStage == 5)
	{
		bool clickedNext =
			mx >= 820 && mx <= 820 + 90 &&
			my >= 520 && my <= 520 + 90;

		if (clickedNext)
		{
			warningFrame++;
			if (warningFrame > 1)
				waitingRoomStage = 6;
		
		}
		return;
	}



	bool clickedCue =
		mx >= cueX && mx <= cueX + cueW &&
		my >= cueY && my <= cueY + cueH;

	if (!clickedCue) return;

	int distance = abs(l3PlayerX - cueX);
	if (distance > 80) return;   // if player far from cue

	if (waitingRoomStage == 0) waitingRoomStage = 1;
	else if (waitingRoomStage == 1) waitingRoomStage = 2;
	else if (waitingRoomStage == 2) waitingRoomStage = 3;
}

#endif