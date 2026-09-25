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

const int crUniformWidth = 100;    
const int crUniformHeight = 100;   

const int crGunWalkOffsetX = 0;    
const int crGunWalkOffsetY = -17;    

const int crGunWalkWidth = 170;   
const int crGunWalkHeight = 110;

const int crGunAttackWidth = 170;     
const int crGunAttackHeight = 120;
const int crGunAttackFrameCount = 2;
const int crGunAttackFrameDelay = 5;

// gun attack alignment (separate from melee's crAttackBackOffset)
const int crGunAttackBackOffset = 40;
const int crGunAttackYOffset = crGunWalkHeight - crGunAttackHeight;   // 130-90 = 40
const int crGunAttackExtraDropY = 4;  
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

// ---------------- Health bar UI ----------------
const int crHealthWidth = 250;
const int crHealthHeight = 30;
const int crHealthY = 520;        // top corner
const int crHealthMargin = 20;
bool crShowUniformMsg = false;
bool crUniformEquipped = false;

bool crShowGunMsg = false;
bool crGunCollected = false;
const int crGunPickupRange = 80;

bool crShowUsbMsg = false;
int crUsbMsgTimer = 0;
const int crUsbMsgDuration = 90;

bool crShowUsbPickupMsg = false;   
const int crMonitorPickupRange = 80;
bool crUsbInserted = false;        // U 
bool crShowUsbSuccess = false;

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
		return;

	int screenPlayerX = crPlayerWorldX - controlRoomWorldX;

	if (crShowUsbMsg)
	{
		iShowImage(screenPlayerX + crGunWalkOffsetX, crPlayerScreenY + crGunWalkOffsetY,
			crGunWalkWidth, crGunWalkHeight, crGunWalkImg[0]);
		return;
	}

	if (crAttacking)
	if (crAttacking)
	{
		if (crGunCollected)
		{
			int gunAttackX = crFacingRight
				? screenPlayerX - crGunAttackBackOffset
				: screenPlayerX + crGunAttackBackOffset;

			int gunAttackY = crPlayerScreenY + crGunWalkOffsetY + crGunAttackYOffset + crGunAttackExtraDropY;

			iShowImage(gunAttackX, gunAttackY, crGunAttackWidth, crGunAttackHeight, crGunAttackImg[crAttackFrame]);
		}
		else
		{
			int attackX = crFacingRight
				? screenPlayerX - crAttackBackOffset
				: screenPlayerX + crAttackBackOffset;

			iShowImage(attackX, crPlayerScreenY, crAttackWidth, crAttackHeight, crAttackImg[crAttackFrame]);
		}
		return;
	}

	if (crGunCollected)
	{
		iShowImage(screenPlayerX + crGunWalkOffsetX, crPlayerScreenY + crGunWalkOffsetY,
			crGunWalkWidth, crGunWalkHeight, crGunWalkImg[crWalkFrame % 6]);
	}
	else if (crUniformEquipped)
	{
		iShowImage(screenPlayerX, crPlayerScreenY, crUniformWidth, crUniformHeight, crUniformWalkImg[crWalkFrame % 8]);
	}
	else if (crFacingRight)
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

// ---------------- Guard 2 (Mask) ----------------
const int crMaskGuardWidth = 66;
const int crMaskGuardHeight = 76;
const int crMaskGuardSpeed = 2;

// 2nd background worldX range = controlRoomScreenWidth to 2*controlRoomScreenWidth
// end edge of 2nd bg:
const int crMaskGuardStartWorldX = 2 * controlRoomScreenWidth - crMaskGuardWidth;
int crMaskGuardWorldX = crMaskGuardStartWorldX;
const int crMaskGuardWorldY = 190;

// stop walking once it reaches start of 2nd bg (don't cross into 1st bg)
int crMaskGuardLeftLimit = controlRoomScreenWidth;
bool crMaskGuardActive = false;
int crMaskGuardWalkFrame = 0;
int crMaskGuardWalkTimer = 0;

bool crMaskGuardDefeated = false;

bool crMaskGuardAttacking = false;
int crMaskGuardAttackFrame = 0;
int crMaskGuardAttackTimer = 0;
const int crMaskGuardAttackFrameCount = 2;      // crMaskGunAttackImg[2] ase, tai 2 frame
const int crMaskGuardAttackFrameDelay = 6;
const int crMaskGuardAttackWidth = 90;          // tomar sprite-er actual size onujayi adjust koro
const int crMaskGuardAttackHeight = 80;

const int crMaskGuardAttackForwardOffset = 30;
const int crMaskGuardAttackDistance = 240;      // ei distance e ashle attack shuru hobe
const int crMaskCombatRange = 240;      // mask guard-only, alada rakhlam

// raw hit counters (proti stage e koto hit lagbe)
int crPlayerRawHitsOnMask = 0;
int crMaskRawHitsOnPlayer = 0;

const int crHitsPerStage = 5;   // every 5 hits = 1 health stage 

// ---------------- Success-screen back button ----------------
const int crBackBtnWidth = 220;
const int crBackBtnHeight = 60;
const int crBackBtnX = (controlRoomScreenWidth - crBackBtnWidth) / 2;   // TODO: adjust position
const int crBackBtnY = 40;                                              // TODO: adjust position



void drawControlRoomProps()
{
	int screenX = crMonitorWorldX - controlRoomWorldX;

	if (screenX > -crMonitorWidth && screenX < controlRoomScreenWidth)
	{
		iShowImage(screenX, crMonitorY, crMonitorWidth, crMonitorHeight, controlRoomMonitorImg);
	}

	if (!crGunCollected)
	{
		int gunScreenX = crGunWorldX - controlRoomWorldX;
		if (gunScreenX > -crGunWidth && gunScreenX < controlRoomScreenWidth)
		{
			iShowImage(gunScreenX, crGunY, crGunWidth, crGunHeight, gunImg);
		}
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

void drawControlRoomHealthBars()
{
	int playerIdx = crGuardHitsOnPlayer;   // player koto mar khaise
	if (playerIdx > 3) playerIdx = 3;

	int guardIdx = crPlayerHitsOnGuard;    // guard koto mar khaise
	if (guardIdx > 3) guardIdx = 3;

	// Left corner = player health
	iShowImage(crHealthMargin, crHealthY, crHealthWidth, crHealthHeight, crHealthImg[playerIdx]);

	// Right corner = guard health
	iShowImage(controlRoomScreenWidth - crHealthWidth - crHealthMargin, crHealthY, crHealthWidth, crHealthHeight, crHealthImg[guardIdx]);
}


void updateControlRoomGuard()
{
	if (!crGuardActive || crGuardDefeated)   // added crGuardDefeated check
		return;
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

void drawMaskGuard()
{
	if (crMaskGuardDefeated || !crGunCollected)
		return;

	int guardScreenX = crMaskGuardWorldX - controlRoomWorldX;

	if (guardScreenX > -crMaskGuardWidth && guardScreenX < controlRoomScreenWidth)
	{
		if (crMaskGuardAttacking)
		{
			int attackScreenX = guardScreenX - crMaskGuardAttackForwardOffset;
			iShowImage(attackScreenX, crMaskGuardWorldY, crMaskGuardAttackWidth, crMaskGuardAttackHeight,
				crMaskGunAttackImg[crMaskGuardAttackFrame]);
		}
		else
		{
			iShowImage(guardScreenX, crMaskGuardWorldY, crMaskGuardWidth, crMaskGuardHeight,
				crMaskWalkImg[crMaskGuardWalkFrame % 5]);
		}
	}
}
void updateMaskGuard()
{
	if (!crGunCollected)
		return;

	if (!crMaskGuardActive && !crMaskGuardDefeated)
	{
		crMaskGuardActive = true;

		crPlayerHitsOnGuard = 0;
		crPlayerRawHitsOnMask = 0;
		crGuardHitsOnPlayer = 0;      // player health bar-o fresh shuru
		crMaskRawHitsOnPlayer = 0;
		crCombatOver = false;
	}

	if (!crMaskGuardActive || crMaskGuardDefeated || crCombatOver)
		return;

	int distance = abs(crPlayerWorldX - crMaskGuardWorldX);

	if (distance <= crMaskGuardAttackDistance)
	{
		if (!crMaskGuardAttacking)
		{
			crMaskGuardAttacking = true;
			crMaskGuardAttackFrame = 0;
			crMaskGuardAttackTimer = 0;
			crGuardHitRegisteredThisSwing = false;
		}

		crMaskGuardAttackTimer++;
		if (crMaskGuardAttackTimer >= crMaskGuardAttackFrameDelay)
		{
			crMaskGuardAttackTimer = 0;
			crMaskGuardAttackFrame++;

			// eituku exactly first guard er moto - ek loop-e ekbar register hobe
			if (crMaskGuardAttackFrame == 1 && !crGuardHitRegisteredThisSwing && !crPlayerDefeated)
			{
				if (distance <= crMaskCombatRange)
				{
					crMaskRawHitsOnPlayer++;             // raw loop count barlo
					crGuardHitRegisteredThisSwing = true;

					// 5 loop hole tobei 1 health stage kombe
					if (crMaskRawHitsOnPlayer % crHitsPerStage == 0)
					{
						crGuardHitsOnPlayer++;
					}

					if (crGuardHitsOnPlayer >= 3)
					{
						crPlayerDefeated = true;
						crCombatOver = true;
					}
				}
			}

			if (crMaskGuardAttackFrame >= crMaskGuardAttackFrameCount)
			{
				crMaskGuardAttackFrame = 0;
				crGuardHitRegisteredThisSwing = false;   // notun loop-er jonno reset — pore loop abar count hobe
			}
		}

		return;
	}
	else
	{
		crMaskGuardAttacking = false;
		crMaskGuardAttackFrame = 0;
	}

	if (crMaskGuardWorldX > crMaskGuardLeftLimit)
	{
		crMaskGuardWorldX -= crMaskGuardSpeed;
	}
	else
	{
		crMaskGuardWorldX = crMaskGuardLeftLimit;
		crMaskGuardActive = false;
	}

	crMaskGuardWalkTimer++;
	if (crMaskGuardWalkTimer >= 6)
	{
		crMaskGuardWalkTimer = 0;
		crMaskGuardWalkFrame++;
		if (crMaskGuardWalkFrame >= 5)
			crMaskGuardWalkFrame = 0;
	}
}

void updateControlRoom()
{
	if (crGuardDefeated && !crUniformEquipped)
	{
		crShowUniformMsg = true;
		crAttacking = false;

		if (isKeyPressed('x') || isKeyPressed('X'))
		{
			crUniformEquipped = true;
			crShowUniformMsg = false;
			crWalkFrame = 0;
			crCombatOver = false;
			crGuardHitsOnPlayer = 0;
		}
	}
	if (crUniformEquipped && !crGunCollected)
	{
		int gunDistance = abs(crPlayerWorldX - crGunWorldX);

		if (gunDistance <= crGunPickupRange)
		{
			crShowGunMsg = true;
			if (isKeyPressed('c') || isKeyPressed('C'))
			{
				crGunCollected = true;
				crShowGunMsg = false;
				crWalkFrame = 0;
			}
		}
		else
		{
			crShowGunMsg = false;
		}
	}
	if (crShowUsbMsg)
	{
		crUsbMsgTimer++;
		if (crUsbMsgTimer >= crUsbMsgDuration)
		{
			crShowUsbMsg = false;
			crCombatOver = false;
			crAttacking = false;
			crAttackFrame = 0;
			crAttackTimer = 0;
			crPlayerHitRegisteredThisSwing = false;
		}
		return;
	}

	if (crMaskGuardDefeated && !crUsbInserted)
	{
		int monitorDistance = abs(crPlayerWorldX - crMonitorWorldX);

		if (monitorDistance <= crMonitorPickupRange)
		{
			crShowUsbPickupMsg = true;

			if (isKeyPressed('u') || isKeyPressed('U'))
			{
				crUsbInserted = true;
				crShowUsbPickupMsg = false;
				crShowUsbSuccess = true;
			}
		}
		else
		{
			crShowUsbPickupMsg = false;
		}
	}

	if (crCombatOver)
		return;


	bool spaceDown = (isKeyPressed(' ') != 0);

	if (crGunCollected)
	{
		// ---- Gun: hold-to-loop attack ----
		if (spaceDown)
		{
			crAttacking = true;
			crAttackTimer++;
			if (crAttackTimer >= crGunAttackFrameDelay)
			{
				crAttackTimer = 0;
				crAttackFrame = (crAttackFrame + 1) % crGunAttackFrameCount;

				// notun loop shuru hoiche - previous swing er reset, space chapa thakleo protibar count hobe
				if (crAttackFrame == 0)
				{
					crPlayerHitRegisteredThisSwing = false;
				}
			}
			if (crAttackFrame == 1 && !crPlayerHitRegisteredThisSwing && !crMaskGuardDefeated)
			{
				int distance = abs(crPlayerWorldX - crMaskGuardWorldX);
				if (distance <= crMaskCombatRange)
				{
					crPlayerRawHitsOnMask++;
					crPlayerHitRegisteredThisSwing = true;

					if (crPlayerRawHitsOnMask % crHitsPerStage == 0)
					{
						crPlayerHitsOnGuard++;
					}

					if (crPlayerHitsOnGuard >= 3)
					{
						crMaskGuardDefeated = true;
						crCombatOver = true;
						crShowUsbMsg = true;
						crUsbMsgTimer = 0;
					}
				}
			}
			updateControlRoomGuard();
			updateMaskGuard();
			return;
		}
		else
		{
			crAttacking = false;
			crAttackFrame = 0;
			crAttackTimer = 0;
			crPlayerHitRegisteredThisSwing = false;
		}
	}
	else
	{

		if (spaceDown && !crAttacking)
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
			updateControlRoomGuard();
			updateMaskGuard();
			return;
		}
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
	updateMaskGuard();
}

	
	

	
void drawControlRoom()
{
	if (crShowUsbSuccess)
	{
		iShowImage(0, 0, controlRoomScreenWidth, 600, usbSuccessImg);
		iShowImage(crBackBtnX, crBackBtnY, crBackBtnWidth, crBackBtnHeight, backBtnImg);
		return;
	}

	drawControlRoomBackground();
	drawControlRoomProps();
	drawControlRoomGuard();
	drawMaskGuard();
	drawControlRoomPlayer();
	drawControlRoomHealthBars();
	if (crShowUniformMsg)
		iShowImage(360, 300, 250, 50, uniformMsgImg);

	if (crShowGunMsg)
		iShowImage(360, 300, 250, 50, gunMsgImg);
	if (crShowUsbMsg)                                     
		iShowImage(360, 300, 250, 50, usbCollectedImg);
	if (crShowUsbPickupMsg)
		iShowImage(360, 300, 250, 50, usbImg);
}


void handleControlRoomClick(int mx, int my)
{

}

#endif