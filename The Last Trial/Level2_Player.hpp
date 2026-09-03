#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "iGraphics.h"

int playerX = 100;
int playerY = 401;

void drawPlayer()
{
	iShowImage(playerX, playerY, 50, 80, playeridle);
}


#endif