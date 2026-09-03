#ifndef LEVEL2_LOGIC_HPP
#define LEVEL2_LOGIC_HPP

#include "iGraphics.h"
#include "Level2_Config.hpp"
#include "Level2_Player.hpp"


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


	// Move right only

	if (GetAsyncKeyState('D') & 0x8000 ||
		GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (playerX >= 400 && playerBgOffset < 4000)
		{
			playerBgOffset += 15;
		}
		else if (playerBgOffset >= 4000 && playerX < 950)
		{
			playerX += 15;
		}
		else if (playerX < 400)
		{
			playerX += 15;
		}
	}
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
}


// =====================================
// BRIDGE FALLING
// =====================================

void updateBridgeFall()
{
	DWORD elapsed = GetTickCount() - bridgeStartTime;


	if (elapsed < 5000)
		return;


	int index = (elapsed - 5000) / 500;


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

	int playerLeft = playerWorldX;
	int playerRight = playerWorldX + 50;

	// Before bridge
	if (playerRight <= 180)
		return true;

	// After bridge
	if (playerLeft >= 4820)
		return true;

	for (int i = 0; i < 45; i++)
	{
		int tileLeft = 180 + i * 110;
		int tileRight = tileLeft + 110;

		// Player is not over this tile
		if (playerRight <= tileLeft ||
			playerLeft >= tileRight)
		{
			continue;
		}

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

	return true;
}


void updatePlayerFall()
{
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