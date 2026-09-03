#ifndef LEVEL2_RENDER_HPP
#define LEVEL2_RENDER_HPP

#include "iGraphics.h"

#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"
#include "Level2_Player.hpp"
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
		int x = i * L2_BG_TILE_WIDTH;

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


void DrawLevel2()
{
	drawPlayerBg();

	drawBotBg();

	drawBridge();

	drawPlayer();
}

#endif