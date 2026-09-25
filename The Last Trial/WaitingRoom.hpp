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
int waitingRoomStage = 6;

int wrTimer = 0;
int takeFrame = 0;
int warningFrame = 0;
bool iKeyWasPressed = false;
// ---------- Level 3 intro slides ----------
const int L3_INTRO_COUNT = 3;
const int L3_FADE_FRAMES = 40;  
const int L3_HOLD_FRAMES = 90;   

bool l3IntroActive = false;
int l3IntroSlide = 0;
int l3IntroTimer = 0;

bool showExploreMsg = false;
int exploreMsgTimer = 0;
const int EXPLORE_MSG_DURATION = 90; 

int exploreMsgX = 350;
int exploreMsgY = 300;
int exploreMsgW = 300;
int exploreMsgH = 60;

void startL3Intro()
{
	l3IntroActive = true;
	l3IntroSlide = 0;
	l3IntroTimer = 0;
}

void updateL3Intro()
{
	if (!l3IntroActive) return;

	l3IntroTimer++;
	if (l3IntroTimer >= L3_FADE_FRAMES + L3_HOLD_FRAMES)
	{
		l3IntroTimer = 0;
		l3IntroSlide++;
		if (l3IntroSlide >= L3_INTRO_COUNT)
		{
			l3IntroActive = false;  

			showExploreMsg = true;
			exploreMsgTimer = 0;
			playLevel3Music();
		}
	}
}


void drawL3Intro()
{
	if (!l3IntroActive) return;

	iShowImage(0, 0, 1000, 600, l3IntroImg[l3IntroSlide]);

	// fade-in
	if (l3IntroTimer < L3_FADE_FRAMES)
	{
		float alpha = 1.0f - (float)l3IntroTimer / L3_FADE_FRAMES;

		glDisable(GL_TEXTURE_2D);   
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f, 0.0f, 0.0f, alpha);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(1000, 0);
		glVertex2f(1000, 600);
		glVertex2f(0, 600);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);   // color reset
	}
}

// ---------- Lamps ----------
const int LAMP_COUNT = 3;
int lampX[LAMP_COUNT] = { 200, 470, 740 };
int lampY = 550;
int lampW = 50;
int lampH = 50;

const int LAMP_SEQ_LEN = 14;
int lampSeq[LAMP_SEQ_LEN] = { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1 };
int lampStep[LAMP_COUNT] = { 0, 0, 00 };
int lampTimer = 0;
int lampSpeed = 10;   // higher = slower

void updateLamps()
{
	lampTimer++;
	if (lampTimer >= lampSpeed)
	{
		lampTimer = 0;
		for (int i = 0; i < LAMP_COUNT; i++)
			lampStep[i] = (lampStep[i] + 1) % LAMP_SEQ_LEN;
	}
}

void drawLamps()
{
	for (int i = 0; i < LAMP_COUNT; i++)
		iShowImage(lampX[i], lampY, lampW, lampH, lampImg[lampSeq[lampStep[i]]]);
}

// ---------- Vase (key) ----------
int vaseX = 245;
int vaseY = 283;
int vaseW = 50;
int vaseH = 37;

bool keyCollected = false;
bool keyMsgShow = false;

// "Key collected" 
int keyMsgX = 320;
int keyMsgY = 300;
int keyMsgW = 300;
int keyMsgH = 50;
// ---------- Door ----------
int doorX = 20;
int doorY = 249;
int doorW = 62;
int doorH = 121;

// "Key fits" image
int keyFitsX = 320;
int keyFitsY = 300;
int keyFitsW = 300;
int keyFitsH = 50;

// 0=none, 1=key fits msg, 2=fade to black, 3=fade in (jungle)
int doorState = 0;
int doorTimer = 0;
const int DOOR_MSG_FRAMES = 90;
const int DOOR_FADE_FRAMES = 40;

void updateExploreMsg()
{
	if (!showExploreMsg) return;

	exploreMsgTimer++;
	if (exploreMsgTimer >= EXPLORE_MSG_DURATION)
	{
		showExploreMsg = false;
		exploreMsgTimer = 0;
	}
}

void drawExploreMsg()
{
	if (!showExploreMsg) return;
	iShowImage(exploreMsgX, exploreMsgY, exploreMsgW, exploreMsgH, exploreImg);
}

void updateDoorTransition()
{
	if (doorState == 0) return;

	doorTimer++;
	if (doorState == 1 && doorTimer >= DOOR_MSG_FRAMES)
	{
		doorState = 2;
		doorTimer = 0;
	}
	else if (doorState == 2 && doorTimer >= DOOR_FADE_FRAMES)
	{
		waitingRoomStage = 6;   // jungle switch 
		doorState = 3;
		doorTimer = 0;
	}
	else if (doorState == 3 && doorTimer >= DOOR_FADE_FRAMES)
	{
		doorState = 0;
		doorTimer = 0;
	}
}

void drawDoorTransition()
{
	if (doorState == 1)
	{
		iShowImage(keyFitsX, keyFitsY, keyFitsW, keyFitsH, keyFitsImg);
	}
	else if (doorState == 2 || doorState == 3)
	{
		float alpha = (float)doorTimer / DOOR_FADE_FRAMES;
		if (doorState == 3) alpha = 1.0f - alpha;

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f, 0.0f, 0.0f, alpha);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(1000, 0);
		glVertex2f(1000, 600);
		glVertex2f(0, 600);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}
void drawWaitingRoomStage()
{   
	drawLamps();
	if (waitingRoomStage < 4 || (waitingRoomStage == 4 && takeFrame < 2))
		iShowImage(cueX, cueY, cueW, cueH, cueImg);

	if (waitingRoomStage == 1)
		iShowImage(l3PlayerX-155, l3PlayerY-20, 250, 400, msg1Img);

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
	if (keyMsgShow)
		iShowImage(keyMsgX, keyMsgY, keyMsgW, keyMsgH, keyMsgImg);
	drawExploreMsg();
}

void updateWaitingRoomStage()
{   
	updateDoorTransition();
	updateLamps();
	updateExploreMsg();
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


	bool iPressed = isKeyPressed('i') || isKeyPressed('I');
	if (waitingRoomStage == 2 && iPressed && !iKeyWasPressed)
	{
		waitingRoomStage = 4;
		takeFrame = 0;
		wrTimer = 0;
	}
	iKeyWasPressed = iPressed;  
}

void handleWaitingRoomClick(int mx, int my)
{
	if (l3IntroActive) return;     // intro slide 
	if (doorState != 0) return;    // door transition 

	// Stage 5: warning "Next" button
	if (waitingRoomStage == 5)
	{
		bool clickedNext =
			mx >= 820 && mx <= 820 + 90 &&
			my >= 520 && my <= 520 + 50;

		if (clickedNext)
		{
			warningFrame++;
			if (warningFrame > 1)
				waitingRoomStage = 7;   // waiting room free roam 
		}
		return;
	}

	// 
	if (keyMsgShow)
	{
		keyMsgShow = false;
		return;
	}

	bool freeRoam = (waitingRoomStage == 0 || waitingRoomStage == 7);

	// Vase click
	if (freeRoam && !keyCollected &&
		mx >= vaseX && mx <= vaseX + vaseW &&
		my >= vaseY && my <= vaseY + vaseH)
	{
		keyCollected = true;
		keyMsgShow = true;
		return;
	}

	// Door click (key thakle)
	if (freeRoam && keyCollected &&
		mx >= doorX && mx <= doorX + doorW &&
		my >= doorY && my <= doorY + doorH)
	{
		doorState = 1;
		doorTimer = 0;
		return;
	}

	// Cue click
	bool clickedCue =
		mx >= cueX && mx <= cueX + cueW &&
		my >= cueY && my <= cueY + cueH;

	if (!clickedCue) return;

	int distance = abs(l3PlayerX - cueX);
	if (distance > 80) return;

	if (waitingRoomStage == 0) waitingRoomStage = 1;
	else if (waitingRoomStage == 1) waitingRoomStage = 2;
}

#endif