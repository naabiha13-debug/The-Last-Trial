#ifndef LEVEL2_RENDER_HPP
#define LEVEL2_RENDER_HPP
#include "iGraphics.h"
#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"

void drawPlayerBg()
{
	float wrapWidth = L2_BG_TILE_COUNT * L2_BG_TILE_WIDTH;

	for (int i = 0; i < L2_BG_TILE_COUNT; i++)
	{
		float xPos = (i * L2_BG_TILE_WIDTH) - playerBgOffset;

		while (xPos < -L2_BG_TILE_WIDTH)
			xPos += wrapWidth;
		while (xPos > L2_SCREEN_WIDTH)
			xPos -= wrapWidth;

		iShowImage((int)xPos, L2_VIEWPORT_HEIGHT, L2_BG_TILE_WIDTH, L2_BG_TILE_HEIGHT, playerBg[i]);
	}
}

void drawBotBg()
{
	float wrapWidth = L2_BG_TILE_COUNT * L2_BG_TILE_WIDTH;

	for (int i = 0; i < L2_BG_TILE_COUNT; i++)
	{
		float xPos = (i * L2_BG_TILE_WIDTH) - botBgOffset;

		while (xPos < -L2_BG_TILE_WIDTH)
			xPos += wrapWidth;
		while (xPos > L2_SCREEN_WIDTH)
			xPos -= wrapWidth;

		iShowImage((int)xPos, 0, L2_BG_TILE_WIDTH, L2_BG_TILE_HEIGHT, botBg[i]);
	}
}

void drawPlayerPlaceholder()
{
	iSetColor(255, 0, 0);
	iFilledRectangle(playerBoxX, playerBoxY, BOX_WIDTH, BOX_HEIGHT);
}

void drawBotPlaceholder()
{
	iSetColor(0, 0, 255);
	iFilledRectangle(botBoxX, botBoxY, BOX_WIDTH, BOX_HEIGHT);
}

void DrawLevel2()
{
	drawPlayerBg();
	drawBotBg();
	drawPlayerPlaceholder();
	drawBotPlaceholder();
}

#endif