#ifndef LEVEL2_BOT_HPP
#define LEVEL2_BOT_HPP

#include "iGraphics.h"
#include "Level2_Config.hpp"
#include "Level2_Logic.hpp"


int botBgOffset = 0;

// ---- Bot run ----
int botX = 100;
int botY = L2_BOT_GROUND_Y;
int botRunFrame = 0;
DWORD botRunFrameTime = 0;
const int BOT_RUN_FRAME_DELAY = 80;
const int BOT_RUN_SPEED = 22;

bool botFalling = false;

// ---- Bot jump ----
bool botJumping = false;
DWORD botJumpStartTime = 0;
DWORD botJumpDuration = 0;
const float BOT_JUMP_HEIGHT = 70.0f;
const int BOT_JUMP_LANDING_MARGIN = 55;
const int BOT_JUMP_LOOKAHEAD_MIN = 40;
const int BOT_JUMP_LOOKAHEAD_MAX = 90;
const int BOT_MISTAKE_CHANCE_PERCENT = 15;
const float BOT_SPEED_PX_PER_MS = (float)BOT_RUN_SPEED / (float)BOT_RUN_FRAME_DELAY;

float botJumpTargetDistance = 0.0f;
float botJumpDistanceCovered = 0.0f;
float botHealth = L2_HEALTH_MAX;
DWORD botHealthDecayTime = 0;

float getBotSpeedFactor()
{
	float factor = botHealth / L2_HEALTH_MAX;
	if (factor < L2_MIN_SPEED_FACTOR)
		factor = L2_MIN_SPEED_FACTOR;
	return factor;
}

void advanceBot(int amount)
{
	if (botX >= 400 && botBgOffset < 4000)
	{
		botBgOffset += amount;
	}
	else if (botBgOffset >= 4000 && botX < 950)
	{
		botX += amount;
	}
	else if (botX < 400)
	{
		botX += amount;
	}
}



bool isGapTile(int i)
{
	if (i < 0 || i >= 45)
		return false;

	return bridgeGap[i] || bridgeState[i] == 2;
}

void startBotJump(int gapTileIndex, bool mistake)
{
	if (botJumping)
		return;

	int botWorldX = botX + botBgOffset;
	int botCenterX = botWorldX + 25;

	int gapEndX = 180 + (gapTileIndex + 1) * 110;
	int targetX = gapEndX + BOT_JUMP_LANDING_MARGIN;

	float distance = (float)(targetX - botCenterX);
	if (distance < 0)
		distance = 0;

	if (mistake)
		distance *= 0.75f;

	botJumpTargetDistance = distance;

	botJumpDuration = (DWORD)(botJumpTargetDistance / BOT_SPEED_PX_PER_MS);
	if (botJumpDuration < 250)
		botJumpDuration = 250;

	botJumpDistanceCovered = 0.0f;
	botJumping = true;
	botJumpStartTime = GetTickCount();
}

void startBotBiscuitJump(int targetBiscuitX)
{
	if (botJumping || botFalling)
		return;

	int botWorldX = botX + botBgOffset;
	int botCenterX = botWorldX + 25;

	int targetX = targetBiscuitX + 50;

	// Make sure bot does not land on a gap tile
	int landingTile = (targetX - 180) / 110;
	while (landingTile >= 0 && landingTile < 45 && isGapTile(landingTile))
	{
		targetX += 110;
		landingTile = (targetX - 180) / 110;
	}

	float distance = (float)(targetX - botCenterX);
	if (distance < 70)
		distance = 70;

	botJumpTargetDistance = distance;
	botJumpDuration = (DWORD)(botJumpTargetDistance / BOT_SPEED_PX_PER_MS);
	if (botJumpDuration < 250)
		botJumpDuration = 250;

	botJumpDistanceCovered = 0.0f;
	botJumping = true;
	botJumpStartTime = GetTickCount();
}

void updateBotBiscuits()
{
	if (botFalling)
		return;

	int botWorldX = botX + botBgOffset;
	int botCenterX = botWorldX + 25;

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (biscuitCollectedByBot[i])
			continue;

		int biscuitCenterX = biscuitWorldX[i] + 15;
		int dist = botCenterX - biscuitCenterX;
		if (dist < 0)
			dist = -dist;

		if (dist <= 65)
		{
			biscuitCollectedByBot[i] = true;
			botHealth += L2_HEALTH_GAIN_AMOUNT;
			if (botHealth > L2_HEALTH_MAX)
				botHealth = L2_HEALTH_MAX;
		}
	}
}

void updateBotJump()
{
	if (botFalling)
		return;

	int botWorldX = botX + botBgOffset;
	int botCenterX = botWorldX + 25;

	if (!botJumping)
	{
		// 1. Auto-jump for biscuit ahead
		for (int b = 0; b < L2_BISCUIT_COUNT; b++)
		{
			if (!biscuitCollectedByBot[b])
			{
				int dist = biscuitWorldX[b] - botCenterX;
				if (dist >= 30 && dist <= 85)
				{
					startBotBiscuitJump(biscuitWorldX[b]);
					return;
				}
			}
		}

		// 2. Check for gap tile
		int lookahead = BOT_JUMP_LOOKAHEAD_MIN +
			rand() % (BOT_JUMP_LOOKAHEAD_MAX - BOT_JUMP_LOOKAHEAD_MIN);

		int aheadX = botCenterX + lookahead;
		int i = (aheadX - 180) / 110;

		if (aheadX > 180 && aheadX < 4820 && isGapTile(i))
		{

			int endIndex = i;
			while (endIndex + 1 < 45 && isGapTile(endIndex + 1))
				endIndex++;

			bool mistake = (rand() % 100) < BOT_MISTAKE_CHANCE_PERCENT;
			startBotJump(endIndex, mistake);
		}
		return;
	}

	DWORD elapsed = GetTickCount() - botJumpStartTime;

	if (elapsed >= botJumpDuration)
	{
		float remaining = botJumpTargetDistance - botJumpDistanceCovered;
		if (remaining > 0)
			advanceBot((int)remaining);

		botJumping = false;
		botY = L2_BOT_GROUND_Y;
		updateBotBiscuits();
		return;
	}

	float targetDistance = botJumpTargetDistance * ((float)elapsed / (float)botJumpDuration);
	float deltaDistance = targetDistance - botJumpDistanceCovered;

	if (deltaDistance >= 1.0f)
	{
		advanceBot((int)deltaDistance);
		botJumpDistanceCovered += (int)deltaDistance;
	}

	float progress = (float)elapsed / (float)botJumpDuration;
	float heightOffset = BOT_JUMP_HEIGHT * sinf(3.14159f * progress);
	botY = L2_BOT_GROUND_Y + (int)heightOffset;

	// Collect biscuits mid-jump
	updateBotBiscuits();
}

void updateBotRun()
{
	if (botFalling)
		return;

	if (botJumping)
		return;

	DWORD elapsed = GetTickCount() - bridgeStartTime;

	if (elapsed < 3000)
		return;

	DWORD now = GetTickCount();


	// =====================================
	// HEALTH DECAY
	// =====================================

	if (now - botHealthDecayTime >= L2_HEALTH_DECAY_INTERVAL)
	{
		botHealthDecayTime = now;

		botHealth -= L2_HEALTH_DECAY_AMOUNT;

		if (botHealth < 0)
			botHealth = 0;
	}


	// =====================================
	// BISCUIT COLLECTION
	// =====================================

	updateBotBiscuits();


	// =====================================
	// BOT MOVEMENT
	// =====================================

	if (now - botRunFrameTime < BOT_RUN_FRAME_DELAY)
		return;

	botRunFrameTime = now;


	int speed = (int)(BOT_RUN_SPEED * getBotSpeedFactor());

	if (speed < 1)
		speed = 1;


	advanceBot(speed);


	botRunFrame = (botRunFrame + 1) % 8;
}
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
	if (botJumping)
		return;

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

	botFalling = true;
	botY -= 5;
	if (botY + 80 < 0)
	{
		return;
	}
}



#endif