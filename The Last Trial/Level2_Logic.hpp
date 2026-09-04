#ifndef LEVEL2_LOGIC_HPP
#define LEVEL2_LOGIC_HPP

#include "iGraphics.h"
#include "Level2_Config.hpp"
#include "Level2_Character.hpp"
#include "Level2_Render.hpp"


int playerBgOffset = 0;



// =====================================
// BRIDGE
// 0 = normal
// 1 = falling
// 2 = gone
// =====================================

int bridgeState[45];

bool bridgeGap[45];

float brokenTileY[45];

int bridgeFallIndex = 0;

DWORD bridgeStartTime;


// BISCUITS (health pickups)
// =====================================
int biscuitWorldX[L2_BISCUIT_COUNT];
bool biscuitCollectedByPlayer[L2_BISCUIT_COUNT];
bool biscuitCollectedByBot[L2_BISCUIT_COUNT];


void initBiscuits()
{
	int span = 4820 - 180;
	int step = span / (L2_BISCUIT_COUNT + 1);

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		biscuitWorldX[i] = 180 + step * (i + 1);
		biscuitCollectedByPlayer[i] = false;
		biscuitCollectedByBot[i] = false;
	}
}


// =====================================
// PLAYER FALLING
// =====================================

bool playerFalling = false;



// =====================================
// PLAYER MOVEMENT
// =====================================

void updateLevel2()
{
	if (playerFalling)
		return;

	DWORD elapsed = GetTickCount() - bridgeStartTime;
	if (elapsed < 3000)
		return;

	DWORD now = GetTickCount();

	// Health decay over time
	if (now - playerHealthDecayTime >= L2_HEALTH_DECAY_INTERVAL)
	{
		playerHealthDecayTime = now;
		playerHealth -= L2_HEALTH_DECAY_AMOUNT;
		if (playerHealth < 0)
			playerHealth = 0;
	}

	// Biscuit collection
	int playerWorldX = playerX + playerBgOffset;
	int playerCenterX = playerWorldX + 25;

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (biscuitCollectedByPlayer[i])
			continue;

		int dist = playerCenterX - biscuitWorldX[i];
		if (dist < 0)
			dist = -dist;

		if (dist <= L2_BISCUIT_CATCH_RANGE)
		{
			biscuitCollectedByPlayer[i] = true;
			playerHealth += L2_HEALTH_GAIN_AMOUNT;
			if (playerHealth > L2_HEALTH_MAX)
				playerHealth = L2_HEALTH_MAX;
			triggerPlayerCatch();
		}
	}

	bool movingRight = (GetAsyncKeyState('D') & 0x8000) ||
		(GetAsyncKeyState(VK_RIGHT) & 0x8000);

	if (movingRight)
	{
		int speed = (int)(L2_BASE_RUN_SPEED * getPlayerSpeedFactor());
		if (speed < 2)
			speed = 2;

		if (playerX >= 400 && playerBgOffset < 4000)
			playerBgOffset += speed;
		else if (playerBgOffset >= 4000 && playerX < 950)
			playerX += speed;
		else if (playerX < 400)
			playerX += speed;
	}

	updatePlayerAnimation(movingRight);
}
// =====================================
// GENERATE RANDOM BRIDGE GAPS
// =====================================

void generateBridgeGaps()
{
	for (int i = 0; i < 45; i++)
	{
		bridgeGap[i] = false;
	}


	for (int i = 3; i < 43; i++)
	{
		if (rand() % 4 == 0 &&
			bridgeGap[i - 1] == false)
		{
			bridgeGap[i] = true;
		}
	}
}


// =====================================
// INITIALIZE BRIDGE
// =====================================

void initBridge()
{
	for (int i = 0; i < 45; i++)
	{
		bridgeState[i] = 0;
		brokenTileY[i] = 394;
	}

	bridgeFallIndex = 0;

	bridgeStartTime = GetTickCount();
	playerFalling = false;
	initBiscuits();
}


// =====================================
// BRIDGE FALLING
// =====================================

void updateBridgeFall()
{
	DWORD elapsed = GetTickCount() - bridgeStartTime;


	if (elapsed < 6000)
		return;


	int index = (elapsed - 6000) / 500;


	if (index >= 45)
		index = 44;


	while (bridgeFallIndex <= index &&
		bridgeFallIndex < 45)
	{
		if (!bridgeGap[bridgeFallIndex])
		{
			bridgeState[bridgeFallIndex] = 1;
		}

		bridgeFallIndex++;
	}
}


// =====================================
// UPDATE FALLING BRIDGE TILES
// =====================================

void updateBrokenTiles()
{
	for (int i = 0; i < 45; i++)
	{
		if (bridgeState[i] == 1)
		{
			brokenTileY[i] -= 5;


			if (brokenTileY[i] <= 300)
			{
				bridgeState[i] = 2;
			}
		}
	}
}


// =====================================
// CHECK PLAYER SUPPORT
// =====================================

bool isPlayerOnBridge()
{
	int playerWorldX = playerX + playerBgOffset;

	// Use the player's center point instead of checking the full
	// sprite width — avoids false "fall" when the player straddles
	// the border between a gap tile and a solid tile after a jump.
	int playerCenterX = playerWorldX + 25;

	// Before bridge
	if (playerCenterX <= 180)
		return true;

	// After bridge
	if (playerCenterX >= 4820)
		return true;

	int i = (playerCenterX - 180) / 110;

	if (i < 0 || i >= 45)
		return true;

	// This position is a permanent gap
	if (bridgeGap[i])
		return false;

	// Tile has completely fallen
	if (bridgeState[i] == 2)
		return false;

	int tileY = 394;

	// Tile is currently falling
	if (bridgeState[i] == 1)
	{
		tileY = (int)brokenTileY[i];
	}

	// Player feet are on the tile
	if (playerY >= tileY &&
		playerY <= tileY + 10)
	{
		return true;
	}

	return false;
}


void updatePlayerFall()
{
	// Player is deliberately airborne (jumping) — don't treat this as falling
	if (playerJumping)
		return;

	int playerWorldX = playerX + playerBgOffset;

	// Before bridge
	if (playerWorldX + 50 <= 180)
	{
		playerFalling = false;
		return;
	}

	// After bridge
	if (playerWorldX >= 4820)
	{
		playerFalling = false;
		return;
	}

	if (isPlayerOnBridge())
	{
		playerFalling = false;
		return;
	}

	// Player is falling
	playerFalling = true;

	playerY -= 5;

	// When the whole player goes below the screen
	if (playerY + 80 < 0)
	{
		playerFalling = false;
	}
}

#endif