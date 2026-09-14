
#ifndef LEVEL3_PLAYER_HPP
#define LEVEL3_PLAYER_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"

// Level 3 Player position
int l3PlayerX = 0;
int l3PlayerY = 245;

// Level 3 Animation
int l3PlayerFrame = 0;
int l3WalkTimer = 0;
bool l3PlayerWalking = false;


// Draw Level 3 Player
void drawLevel3Player()
{
	if (l3PlayerFrame == 0)
		iShowImage(l3PlayerX, l3PlayerY, 70, 105, l3idleImg);

	else if (l3PlayerFrame == 1)
		iShowImage(l3PlayerX, l3PlayerY, 100, 105, l3walk1Img);

	else if (l3PlayerFrame == 2)
		iShowImage(l3PlayerX, l3PlayerY, 100, 105, l3walk2Img);

	else if (l3PlayerFrame == 3)
		iShowImage(l3PlayerX, l3PlayerY, 100, 105, l3walk3Img);

	else if (l3PlayerFrame == 4)
		iShowImage(l3PlayerX, l3PlayerY, 100, 105, l3walk4Img);
}
void updateLevel3Player()
{
	// Right arrow
	if (isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		l3PlayerX += 5;
		l3PlayerWalking = true;

		l3WalkTimer++;

		if (l3WalkTimer >= 5)
		{
			l3WalkTimer = 0;
			l3PlayerFrame++;

			if (l3PlayerFrame < 3 || l3PlayerFrame > 4)
				l3PlayerFrame = 3;
		}
	}

	// Left arrow
	else if (isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		l3PlayerX -= 5;
		l3PlayerWalking = true;

		l3WalkTimer++;

		if (l3WalkTimer >= 5)
		{
			l3WalkTimer = 0;
			l3PlayerFrame++;

			if (l3PlayerFrame < 1 || l3PlayerFrame > 2)
				l3PlayerFrame = 1;
		}
	}

	// No movement 
	else
	{
		l3PlayerWalking = false;
		l3WalkTimer = 0;
		l3PlayerFrame = 0;
	}
}
#endif