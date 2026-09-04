#ifndef LEVEL2_LOGIC_HPP
#define LEVEL2_LOGIC_HPP

#include "iGraphics.h"
#include "Level2_Config.hpp"
#include "Level2_Character.hpp"


int playerBgOffset = 0;

// Bot's own scroll offset (bot will move itself later — for now stays 0)
int botBgOffset = 0;


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
// HEALTH SYSTEM
// =====================================

float playerHealth = L2_HEALTH_MAX;
float botHealth = L2_HEALTH_MAX;

DWORD playerHealthTimer;
DWORD botHealthTimer;

int biscuitWorldX[L2_BISCUIT_COUNT] = { 700, 1500, 2300, 3100, 3700, 4400 };

bool playerBiscuitCollected[L2_BISCUIT_COUNT];
bool botBiscuitCollected[L2_BISCUIT_COUNT];

void initHealthSystem()
{
	playerHealth = L2_HEALTH_MAX;
	botHealth = L2_HEALTH_MAX;

	playerHealthTimer = GetTickCount();
	botHealthTimer = GetTickCount();

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		playerBiscuitCollected[i] = false;
		botBiscuitCollected[i] = false;
	}
}

int getPlayerRunSpeed()
{
	float factor = playerHealth / L2_HEALTH_MAX;

	if (factor < L2_MIN_SPEED_FACTOR)
		factor = L2_MIN_SPEED_FACTOR;

	return (int)(L2_BASE_RUN_SPEED * factor);
}

int getBotRunSpeed()
{
	float factor = botHealth / L2_HEALTH_MAX;

	if (factor < L2_MIN_SPEED_FACTOR)
		factor = L2_MIN_SPEED_FACTOR;

	return (int)(L2_BASE_RUN_SPEED * factor);
}

void updateHealthDecay()
{
	DWORD now = GetTickCount();

	if (now - playerHealthTimer >= L2_HEALTH_DECAY_INTERVAL)
	{
		playerHealthTimer = now;
		playerHealth -= L2_HEALTH_DECAY_AMOUNT;

		if (playerHealth < 0)
			playerHealth = 0;
	}

	if (now - botHealthTimer >= L2_HEALTH_DECAY_INTERVAL)
	{
		botHealthTimer = now;
		botHealth -= L2_HEALTH_DECAY_AMOUNT;

		if (botHealth < 0)
			botHealth = 0;
	}
}


// =====================================
// BOT (the Level 1 winner) — auto-runs in its own screen
// =====================================

int botX = 100;
int botY = L2_BOT_GROUND_Y;
int botRunFrame = 0;
DWORD botRunFrameTime = 0;
const int BOT_RUN_FRAME_DELAY = 80;
const int BOT_RUN_SPEED = 15;

bool botFalling = false;

void updateBotRun()
{
	if (botFalling)
		return;

	DWORD now = GetTickCount();
	if (now - botRunFrameTime < BOT_RUN_FRAME_DELAY)
		return;

	botRunFrameTime = now;

	int speed = getBotRunSpeed();

	if (botX >= 400 && botBgOffset < 4000)
	{
		botBgOffset += speed;
	}
	else if (botBgOffset >= 4000 && botX < 950)
	{
		botX += speed;
	}
	else if (botX < 400)
	{
		botX += speed;
	}

	botRunFrame = (botRunFrame + 1) % 8;
}

void checkPlayerBiscuitCatch()
{
	int playerWorldX = playerX + playerBgOffset + 25; // character center

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (playerBiscuitCollected[i])
			continue;

		if (abs(playerWorldX - biscuitWorldX[i]) <= L2_BISCUIT_CATCH_RANGE)
		{
			playerBiscuitCollected[i] = true;

			playerHealth += L2_HEALTH_GAIN_AMOUNT;
			if (playerHealth > L2_HEALTH_MAX)
				playerHealth = L2_HEALTH_MAX;
		}
	}
}


void checkBotBiscuitCatch()
{
	int botWorldX = botX + botBgOffset + 25;

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (botBiscuitCollected[i])
			continue;

		if (abs(botWorldX - biscuitWorldX[i]) <= L2_BISCUIT_CATCH_RANGE)
		{
			botBiscuitCollected[i] = true;

			botHealth += L2_HEALTH_GAIN_AMOUNT;
			if (botHealth > L2_HEALTH_MAX)
				botHealth = L2_HEALTH_MAX;
		}
	}
}

// =====================================
// BOT FALLING (bot doesn't jump — falls through gaps)
// =====================================

bool isBotOnBridge()
{
	int botWorldX = botX + botBgOffset;
	int botCenterX = botWorldX + 25;

	// Before bridge
	if (botCenterX <= 180)
		return true;

	// After bridge
	if (botCenterX >= 4820)
		return true;

	int i = (botCenterX - 180) / 110;

	if (i < 0 || i >= 45)
		return true;

	// Permanent gap
	if (bridgeGap[i])
		return false;

	// Tile has completely fallen
	if (bridgeState[i] == 2)
		return false;

	// Same tile, mirrored onto the bot's side (matches drawBridgeBot)
	int playerTileY = L2_PLAYER_TILE_Y;

	if (bridgeState[i] == 1)
		playerTileY = (int)brokenTileY[i];

	int fallenAmount = L2_PLAYER_TILE_Y - playerTileY;
	int tileY = L2_BOT_TILE_Y - fallenAmount;

	if (botY >= tileY && botY <= tileY + 10)
	{
		return true;
	}

	return false;
}


void updateBotFall()
{
	int botWorldX = botX + botBgOffset;

	// Before bridge
	if (botWorldX + 50 <= 180)
	{
		botFalling = false;
		return;
	}

	// After bridge
	if (botWorldX >= 4820)
	{
		botFalling = false;
		return;
	}

	if (isBotOnBridge())
	{
		botFalling = false;
		return;
	}

	// Bot is falling
	botFalling = true;

	botY -= 5;

	if (botY + 80 < 0)
	{
		botFalling = false;
	}
}


// =====================================
// PLAYER MOVEMENT
// =====================================

void updateLevel2()
{
	if (playerFalling)
		return;

	bool movingRight = (GetAsyncKeyState('D') & 0x8000) ||
		(GetAsyncKeyState(VK_RIGHT) & 0x8000);

	int speed = getPlayerRunSpeed();

	if (movingRight)
	{
		if (playerX >= 400 && playerBgOffset < 4000)
		{
			playerBgOffset += speed;
		}
		else if (playerBgOffset >= 4000 && playerX < 950)
		{
			playerX += speed;
		}
		else if (playerX < 400)
		{
			playerX += speed;
		}
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