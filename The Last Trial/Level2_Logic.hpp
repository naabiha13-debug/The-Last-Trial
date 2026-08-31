#ifndef LEVEL2_LOGIC_HPP
#define LEVEL2_LOGIC_HPP
#include "iGraphics.h"
#include "Level2_Config.hpp"

void level2Update()
{
	// Player box control: A/D (left/right)
	if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		playerBoxX += PLAYER_MOVE_SPEED;
		playerBgOffset += PLAYER_SCROLL_SPEED;
	}
	if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		playerBoxX -= PLAYER_MOVE_SPEED;
		playerBgOffset -= PLAYER_SCROLL_SPEED;
	}

	// Bot box control (temporary test key): J/L
	if (isKeyPressed('l'))
	{
		botBoxX += BOT_MOVE_SPEED;
		botBgOffset += BOT_SCROLL_SPEED;
	}
	if (isKeyPressed('j'))
	{
		botBoxX -= BOT_MOVE_SPEED;
		botBgOffset -= BOT_SCROLL_SPEED;
	}
}

#endif