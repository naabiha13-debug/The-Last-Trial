#ifndef LEVEL2_RENDER_HPP
#define LEVEL2_RENDER_HPP

#include "iGraphics.h"

#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"
#include "Level2_Character.hpp"
#include "Level2_Logic.hpp"


char tickerText[] = "  THE TRIAL IS GOING ON - YOU VS THE PLAYER - A SINGLE MISSTEP WILL BE YOUR LAST.     ";

int l2TextPixelWidth(char *str, void *font)
{
	int width = 0;

	for (char *ch = str; *ch != '\0'; ch++)
		width += glutBitmapWidth(font, (int)(unsigned char)*ch);

	return width;
}


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

void drawScreenPartition()
{
	int barY = 293;
	int barH = 34;

	// Black partition
	iSetColor(0, 0, 0);
	iFilledRectangle(0, barY, L2_SCREEN_WIDTH, barH);

	// Fixed centered text
	void *tickerFont = GLUT_BITMAP_9_BY_15;

	int textWidth = l2TextPixelWidth(tickerText, tickerFont);

	int textX = (L2_SCREEN_WIDTH - textWidth) / 2;
	int textY = barY + (barH - 15) / 2;

	iSetColor(255, 255, 255);
	iText(textX, textY, tickerText, tickerFont);
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
	// Clip to the bot's own viewport (y: 0-300) so it never bleeds
	// into the player's screen
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	// qualifiedBot comes from Level 1 (0 = Bot 047, 1 = Bot 392) —
	// whichever bot qualified alongside the player runs here
	int img = botRunImg[qualifiedBot][botRunFrame];

	iShowImage(botX, L2_BOT_GROUND_Y, 50, 80, img);

	glDisable(GL_SCISSOR_TEST);
}

void drawBiscuits()
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, L2_VIEWPORT_HEIGHT, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (playerBiscuitCollected[i])
			continue;

		int screenX = biscuitWorldX[i] - playerBgOffset;
		iShowImage(screenX, 460, 30, 30, biscuitImg);
	}

	glDisable(GL_SCISSOR_TEST);
}


void drawBiscuitsBot()
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	for (int i = 0; i < L2_BISCUIT_COUNT; i++)
	{
		if (botBiscuitCollected[i])
			continue;

		int screenX = biscuitWorldX[i] - botBgOffset;
		iShowImage(screenX, 150, 30, 30, biscuitImg);
	}

	glDisable(GL_SCISSOR_TEST);
}


void drawHealthBars()
{
	int barWidth = 200;
	int barHeight = 18;

	// Player HP bar
	int playerBarX = 20, playerBarY = 560;
	iSetColor(60, 60, 60);
	iFilledRectangle(playerBarX, playerBarY, barWidth, barHeight);
	int playerFill = (int)(barWidth * (playerHealth / L2_HEALTH_MAX));
	iSetColor(0, 200, 0);
	iFilledRectangle(playerBarX, playerBarY, playerFill, barHeight);
	iSetColor(255, 255, 255);
	iText(playerBarX, playerBarY + barHeight + 12, "PLAYER HP", GLUT_BITMAP_9_BY_15);

	// Bot HP bar
	int botBarX = 20, botBarY = 270;
	iSetColor(60, 60, 60);
	iFilledRectangle(botBarX, botBarY, barWidth, barHeight);
	int botFill = (int)(barWidth * (botHealth / L2_HEALTH_MAX));
	iSetColor(200, 0, 0);
	iFilledRectangle(botBarX, botBarY, botFill, barHeight);
	iSetColor(255, 255, 255);
	iText(botBarX, botBarY + barHeight + 12, "BOT HP", GLUT_BITMAP_9_BY_15);
}

void DrawLevel2()
{
	drawPlayerBg();

	drawBotBg();

	drawBridge();

	drawBiscuits();

	drawBiscuitsBot();

	drawScreenPartition();

	drawBridgeBot();

	drawBot();

	drawPlayer();
}

#endif