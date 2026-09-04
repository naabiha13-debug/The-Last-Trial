#ifndef LEVEL2_RENDER_HPP
#define LEVEL2_RENDER_HPP

#include "iGraphics.h"

#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"
#include "Level2_Character.hpp"
#include "Level2_Logic.hpp"
#include "Level2_Bot.hpp"


void drawPlayerBg()
{
	for (int i = 0; i < L2_BG_TILE_COUNT; i++)
	{
		int x = i * L2_BG_TILE_WIDTH - playerBgOffset;

		iShowImage(
			x,
			300,
			L2_BG_TILE_WIDTH,
			L2_BG_TILE_HEIGHT,
			playerBg[i]
			);
	}
}


void drawBotBg()
{
	for (int i = 0; i < L2_BG_TILE_COUNT; i++)
	{
		int x = i * L2_BG_TILE_WIDTH - botBgOffset;

		iShowImage(
			x,
			0,
			L2_BG_TILE_WIDTH,
			L2_BG_TILE_HEIGHT,
			botBg[i]
			);
	}
}


void drawBridge()
{
	int startX = 180;
	int bridgeEnd = 5000 - 180;
	int tileWidth = 110;

	for (int i = 0;; i++)
	{
		int worldX = startX + i * tileWidth;

		if (worldX >= bridgeEnd)
			break;

		if (bridgeGap[i])
			continue;

		if (bridgeState[i] == 2)
			continue;

		int screenX = worldX - playerBgOffset;

		int tileY;

		if (bridgeState[i] == 1)
			tileY = (int)brokenTileY[i];
		else
			tileY = 394;

		int currentWidth = tileWidth;

		if (worldX + currentWidth > bridgeEnd)
		{
			currentWidth = bridgeEnd - worldX;
		}

		iShowImage(
			screenX,
			tileY,
			currentWidth,
			7,
			bridgeTile
			);
	}
}


void drawBridgeBot()
{
	int startX = 180;
	int bridgeEnd = 5000 - 180;
	int tileWidth = 110;

	for (int i = 0;; i++)
	{
		int worldX = startX + i * tileWidth;

		if (worldX >= bridgeEnd)
			break;

		if (bridgeGap[i])
			continue;

		if (bridgeState[i] == 2)
			continue;

		int screenX = worldX - botBgOffset;

		int playerTileY;

		if (bridgeState[i] == 1)
			playerTileY = (int)brokenTileY[i];
		else
			playerTileY = L2_PLAYER_TILE_Y;

		// Mirror the fall amount onto the bot's side
		int fallenAmount = L2_PLAYER_TILE_Y - playerTileY;
		int tileY = L2_BOT_TILE_Y - fallenAmount;

		int currentWidth = tileWidth;

		if (worldX + currentWidth > bridgeEnd)
		{
			currentWidth = bridgeEnd - worldX;
		}

		iShowImage(
			screenX,
			tileY,
			currentWidth,
			7,
			bridgeTile
			);
	}
}

void drawBot()
{
	if (botFalling && botY + 80 < 0)
		return;

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	int img;

	if (botFalling)
	{
		int fallFrame = (GetTickCount() / 120) % 3;
		img = botFallImg[qualifiedBot][fallFrame];
	}
	else if (botJumping)
	{
		DWORD elapsed = GetTickCount() - botJumpStartTime;
		DWORD frameDelay = botJumpDuration / 5;
		if (frameDelay == 0)
			frameDelay = 1;

		int jumpFrame = elapsed / frameDelay;
		if (jumpFrame > 4)
			jumpFrame = 4;

		img = botJumpImg[qualifiedBot][jumpFrame];
	}
	else
	{
		img = botRunImg[qualifiedBot][botRunFrame];
	}

	iShowImage(botX, botY, 50, 80, img);

	glDisable(GL_SCISSOR_TEST);
}

void drawBiscuitsPlayer()
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, L2_VIEWPORT_HEIGHT, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (biscuitCollectedByPlayer[i])
			continue;

		int screenX = biscuitWorldX[i] - playerBgOffset;
		int screenY = PLAYER_GROUND_Y + L2_BISCUIT_HEIGHT_OFFSET;

		iShowImage(screenX, screenY, 30, 30, biscuitImg);
	}

	glDisable(GL_SCISSOR_TEST);
}

void drawBiscuitsBot()
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (biscuitCollectedByBot[i])
			continue;

		int screenX = biscuitWorldX[i] - botBgOffset;
		int screenY = L2_BOT_GROUND_Y + L2_BISCUIT_HEIGHT_OFFSET;

		iShowImage(screenX, screenY, 30, 30, biscuitImg);
	}

	glDisable(GL_SCISSOR_TEST);
}

void drawLevel2HUD()
{
	// =========================================
	// PLAYER HEALTH - UPPER SCREEN
	// =========================================

	int boxX = 20;
	int boxY = 535;
	int boxW = 220;
	int boxH = 48;

	// Dark purple background
	iSetColor(35, 20, 45);
	iFilledRectangle(boxX, boxY, boxW, boxH);

	// Dark purple outer border
	iSetColor(75, 45, 95);
	iRectangle(boxX - 2, boxY - 2, boxW + 4, boxH + 4);

	// Lavender border
	iSetColor(190, 160, 220);
	iRectangle(boxX, boxY, boxW, boxH);

	// Title
	iSetColor(200, 175, 225);

	iText(
		boxX + 12,
		boxY + boxH - 17,
		"457 HEALTH",
		GLUT_BITMAP_8_BY_13);


	// Health value
	char playerHealthText[32];

	sprintf_s(
		playerHealthText,
		sizeof(playerHealthText),
		"%d / %d",
		(int)playerHealth,
		(int)L2_HEALTH_MAX);


	iSetColor(235, 220, 245);

	iText(
		boxX + 12,
		boxY + 10,
		playerHealthText,
		GLUT_BITMAP_8_BY_13);


	// Health bar
	int barX = boxX + 90;
	int barY = boxY + 11;
	int barW = 115;
	int barH = 14;

	// Empty bar
	iSetColor(15, 10, 20);
	iFilledRectangle(
		barX,
		barY,
		barW,
		barH);

	// Filled health
	int playerBarW =
		(int)((playerHealth / L2_HEALTH_MAX) * barW);

	if (playerBarW < 0)
		playerBarW = 0;

	if (playerBarW > barW)
		playerBarW = barW;

	if (playerHealth > 0)
	{
		iSetColor(190, 160, 220);

		iFilledRectangle(
			barX,
			barY,
			playerBarW,
			barH);
	}

	// Bar border
	iSetColor(200, 175, 225);

	iRectangle(
		barX,
		barY,
		barW,
		barH);


	// =========================================
	// BOT HEALTH - LOWER SCREEN
	// =========================================

	boxX = 20;
	boxY = 235;
	boxW = 220;
	boxH = 48;

	// Black background
	iSetColor(8, 8, 8);

	iFilledRectangle(
		boxX,
		boxY,
		boxW,
		boxH);

	// Dark orange outer border
	iSetColor(110, 55, 10);

	iRectangle(
		boxX - 2,
		boxY - 2,
		boxW + 4,
		boxH + 4);

	// Orange border
	iSetColor(255, 140, 30);

	iRectangle(
		boxX,
		boxY,
		boxW,
		boxH);


	// Title
	iSetColor(255, 140, 30);

	iText(
		boxX + 12,
		boxY + boxH - 17,
		"BOT HEALTH",
		GLUT_BITMAP_8_BY_13);


	// Health value
	char botHealthText[32];

	sprintf_s(
		botHealthText,
		sizeof(botHealthText),
		"%d / %d",
		(int)botHealth,
		(int)L2_HEALTH_MAX);


	iSetColor(255, 190, 90);

	iText(
		boxX + 12,
		boxY + 10,
		botHealthText,
		GLUT_BITMAP_8_BY_13);


	// Health bar
	barX = boxX + 90;
	barY = boxY + 11;
	barW = 115;
	barH = 14;

	// Empty bar
	iSetColor(15, 15, 15);

	iFilledRectangle(
		barX,
		barY,
		barW,
		barH);


	// Filled health
	int botBarW =
		(int)((botHealth / L2_HEALTH_MAX) * barW);

	if (botBarW < 0)
		botBarW = 0;

	if (botBarW > barW)
		botBarW = barW;

	if (botHealth > 0)
	{
		iSetColor(255, 140, 30);

		iFilledRectangle(
			barX,
			barY,
			botBarW,
			barH);
	}

	// Bar border
	iSetColor(255, 140, 30);

	iRectangle(
		barX,
		barY,
		barW,
		barH);
}

void DrawLevel2()
{
	drawPlayerBg();
	drawBotBg();

	drawBridge();
	drawBridgeBot();

	drawBiscuitsPlayer();
	drawBiscuitsBot();

	drawBot();
	drawPlayer();

	drawLevel2HUD();
}

#endif