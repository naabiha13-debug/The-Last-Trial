#ifndef LEVEL2_RENDER_HPP
#define LEVEL2_RENDER_HPP

#include "iGraphics.h"

#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"
#include "Level2_Character.hpp"
#include "Level2_Logic.hpp"


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


void DrawLevel2()
{
	drawPlayerBg();

	drawBotBg();

	drawBridge();

	drawBridgeBot();

	drawBot();

	drawPlayer();
}

#endif