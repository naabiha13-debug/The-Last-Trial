#ifndef CONTROL_ROOM_HPP
#define CONTROL_ROOM_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"

// ---------------- World / camera ----------------
const int controlRoomScreenWidth = 1000;
const int controlRoomTotalWidth = 3 * controlRoomScreenWidth;   
const int controlRoomMaxScroll = controlRoomTotalWidth - controlRoomScreenWidth;

int controlRoomWorldX = 0;
const int controlRoomMiddleX = 465;   

// ---------------- Player ----------------
int crPlayerWorldX = 0;
const int crPlayerScreenY = 190;      

bool crFacingRight = true;
int crWalkFrame = 0;
int crWalkTimer = 0;

const int crPlayerWidth = 60;
const int crPlayerHeight = 80;

bool crAttacking = false;
int crAttackFrame = 0;
int crAttackTimer = 0;
const int crAttackWidth = 180;   
const int crAttackHeight = 90;  
const int crAttackFrameCount = 6;
const int crAttackFrameDelay = 5;   
const int crAttackBackOffset = 70;  

// ---------------- Combat ----------------
const int crCombatRange = 110;
const int crHitRegisterFrame = 3;

int crPlayerHitsOnGuard = 0;
int crGuardHitsOnPlayer = 0;

bool crPlayerHitRegisteredThisSwing = false;
bool crGuardHitRegisteredThisSwing = false;

bool crGuardDefeated = false;
bool crPlayerDefeated = false;

bool crCombatOver = false;

void drawControlRoomBackground()
{
	for (int i = 0; i < 3; i++)
	{
		int bgScreenX = (i * controlRoomScreenWidth) - controlRoomWorldX;
		iShowImage(bgScreenX, 0, controlRoomScreenWidth, 600, controlRoomBgImg[i]);
	}
}

void drawControlRoomPlayer()
{   
	if (crPlayerDefeated)
		return;   // invisible

	int screenPlayerX = crPlayerWorldX - controlRoomWorldX;

	if (crAttacking)
	{
		int attackX = crFacingRight
			? screenPlayerX - crAttackBackOffset
			: screenPlayerX + crAttackBackOffset;

		iShowImage(attackX, crPlayerScreenY, crAttackWidth, crAttackHeight, crAttackImg[crAttackFrame]);
		return;
	}

	if (crFacingRight)
		iShowImage(screenPlayerX, crPlayerScreenY, crPlayerWidth, crPlayerHeight, l3walkFImg[crWalkFrame]);
	else
		iShowImage(screenPlayerX, crPlayerScreenY, crPlayerWidth, crPlayerHeight, l3walkBImg[crWalkFrame]);
}





const int crMonitorWorldX = 2790;   
const int crMonitorY = 242;         
const int crMonitorWidth = 50;    
const int crMonitorHeight = 30;  

const int crGunWorldX = 1005;   
const int crGunY = 190;
const int crGunWidth = crPlayerWidth;
const int crGunHeight = crPlayerHeight;

// ---------------- Guard (bot) ----------------
const int crGuardWidth = 80;
const int crGuardHeight = 90;
const int crGuardSpeed = 2;


int crGuardWorldX = controlRoomScreenWidth - crGuardWidth; 
const int crGuardWorldY = 190;

int crGuardLeftLimit = 0;

int crGuardWalkFrame = 0;
int crGuardWalkTimer = 0;
bool crGuardActive = true;  

bool crGuardAttacking = false;
int crGuardAttackFrame = 0;
int crGuardAttackTimer = 0;
const int crGuardAttackFrameCount = 5;
const int crGuardAttackFrameDelay = 6;
const int crGuardAttackWidth = 90;
const int crGuardAttackHeight = 80;

const int crGuardAttackForwardOffset = 30;   

const int crGuardAttackDistance = 100;


void drawControlRoomProps()
{
	int screenX = crMonitorWorldX - controlRoomWorldX;

	if (screenX > -crMonitorWidth && screenX < controlRoomScreenWidth)
	{
		iShowImage(screenX, crMonitorY, crMonitorWidth, crMonitorHeight, controlRoomMonitorImg);
	}

	int gunScreenX = crGunWorldX - controlRoomWorldX;
	if (gunScreenX > -crGunWidth && gunScreenX < controlRoomScreenWidth)
	{
		iShowImage(gunScreenX, crGunY, crGunWidth, crGunHeight, gunImg);
	}
}

void drawControlRoomGuard()
{
	if (crGuardDefeated)
		return;
	int guardScreenX = crGuardWorldX - controlRoomWorldX;

	if (guardScreenX > -crGuardWidth && guardScreenX < controlRoomScreenWidth)
	{
		if (crGuardAttacking)
		{
			
			int attackScreenX = guardScreenX - crGuardAttackForwardOffset;

			iShowImage(attackScreenX, crGuardWorldY, crGuardAttackWidth, crGuardAttackHeight, guardAttackImg[crGuardAttackFrame]);
		}
		else
		{
			iShowImage(guardScreenX, crGuardWorldY, crGuardWidth, crGuardHeight, guardWalkImg[crGuardWalkFrame]);
		}
	}
}


void updateControlRoomGuard()
{
	if (!crGuardActive || crCombatOver)
		return;

	int distance = abs(crPlayerWorldX - crGuardWorldX);

	if (distance <= crGuardAttackDistance)
	{
		if (!crGuardAttacking)
		{
			crGuardAttacking = true;
			crGuardAttackFrame = 0;
			crGuardAttackTimer = 0;
			crGuardHitRegisteredThisSwing = false;
		}

		crGuardAttackTimer++;
		if (crGuardAttackTimer >= crGuardAttackFrameDelay)
		{
			crGuardAttackTimer = 0;
			crGuardAttackFrame++;

			if (crGuardAttackFrame == crHitRegisterFrame && !crGuardHitRegisteredThisSwing && !crPlayerDefeated)
			{
				if (distance <= crCombatRange)
				{
					crGuardHitsOnPlayer++;
					crGuardHitRegisteredThisSwing = true;

					if (crGuardHitsOnPlayer >= 3)
					{
						crPlayerDefeated = true;
						crCombatOver = true;
					}
				}
			}

			if (crGuardAttackFrame >= crGuardAttackFrameCount)
			{
				crGuardAttackFrame = 0;
				crGuardHitRegisteredThisSwing = false; 
			}
		}

		return;
	}
	else
	{
		crGuardAttacking = false;
		crGuardAttackFrame = 0;
	}

	if (crGuardWorldX > crGuardLeftLimit)
	{
		crGuardWorldX -= crGuardSpeed;
	}
	else
	{
		crGuardWorldX = crGuardLeftLimit;
		crGuardActive = false;
	}

	crGuardWalkTimer++;
	if (crGuardWalkTimer >= 6)
	{
		crGuardWalkTimer = 0;
		crGuardWalkFrame++;
		if (crGuardWalkFrame >= 4)
			crGuardWalkFrame = 0;
	}
}

void updateControlRoom()
{
	if (crCombatOver)
		return;

	if (isKeyPressed(' ') && !crAttacking)
	{
		crAttacking = true;
		crAttackFrame = 0;
		crAttackTimer = 0;
		crPlayerHitRegisteredThisSwing = false;
	}

	if (crAttacking)
	{
		crAttackTimer++;
		if (crAttackTimer >= crAttackFrameDelay)
		{
			crAttackTimer = 0;
			crAttackFrame++;

			if (crAttackFrame == crHitRegisterFrame && !crPlayerHitRegisteredThisSwing && !crGuardDefeated)
			{
				int distance = abs(crPlayerWorldX - crGuardWorldX);
				if (distance <= crCombatRange)
				{
					crPlayerHitsOnGuard++;
					crPlayerHitRegisteredThisSwing = true;

					if (crPlayerHitsOnGuard >= 3)
					{
						crGuardDefeated = true;
						crCombatOver = true;
					}
				}
			}

			if (crAttackFrame >= crAttackFrameCount)
			{
				crAttacking = false;
				crAttackFrame = 0;
			}
		}
		return;
	}


	bool isMoving = isSpecialKeyPressed(GLUT_KEY_RIGHT) || isSpecialKeyPressed(GLUT_KEY_LEFT);

	if (isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		crFacingRight = true;

		if (crPlayerWorldX < controlRoomTotalWidth - crPlayerWidth)
			crPlayerWorldX += 5;

		if ((crPlayerWorldX - controlRoomWorldX) > controlRoomMiddleX && controlRoomWorldX < controlRoomMaxScroll)
			controlRoomWorldX += 5;

		if (controlRoomWorldX > controlRoomMaxScroll)
			controlRoomWorldX = controlRoomMaxScroll;
	}
	else if (isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		crFacingRight = false;

		if (crPlayerWorldX > 0)
			crPlayerWorldX -= 5;

		if ((crPlayerWorldX - controlRoomWorldX) < controlRoomMiddleX && controlRoomWorldX > 0)
			controlRoomWorldX -= 5;

		if (controlRoomWorldX < 0)
			controlRoomWorldX = 0;
	}

	if (isMoving)
	{
		crWalkTimer++;
		if (crWalkTimer >= 5)
		{
			crWalkTimer = 0;
			crWalkFrame++;
			if (crWalkFrame > 8)
				crWalkFrame = 0;
		}
	}
	else
	{
		crWalkTimer = 0;
		crWalkFrame = 0;
	}

	updateControlRoomGuard();   
}
void drawControlRoom()
{
	drawControlRoomBackground();
	drawControlRoomProps();
	drawControlRoomGuard();
	drawControlRoomPlayer();
}


void handleControlRoomClick(int mx, int my)
{
	
}

#endif