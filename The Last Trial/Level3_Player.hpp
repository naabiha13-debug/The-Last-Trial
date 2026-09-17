
#ifndef LEVEL3_PLAYER_HPP
#define LEVEL3_PLAYER_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"

// Level 3 Player position
int l3PlayerX = 0;
int l3PlayerY = 242;

// Level 3 Animation
int l3PlayerFrame = 0;      // 0 to 8 (9 frames)
int l3WalkTimer = 0;
bool l3PlayerWalking = false;
bool l3FacingRight = true;  // true = right/forward, false = left/backward


// Draw Level 3 Player
void drawLevel3Player()
{
	if (l3FacingRight)
		iShowImage(l3PlayerX, l3PlayerY, 70, 90, l3walkFImg[l3PlayerFrame]);
	else
		iShowImage(l3PlayerX, l3PlayerY, 70, 90, l3walkBImg[l3PlayerFrame]);
}

void updateLevel3Player()
{
	// Right arrow
	if (isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		l3PlayerX += 5;
		l3PlayerWalking = true;
		l3FacingRight = true;

		l3WalkTimer++;

		if (l3WalkTimer >= 5)
		{
			l3WalkTimer = 0;
			l3PlayerFrame++;

			if (l3PlayerFrame > 8)
				l3PlayerFrame = 0;
		}
	}

	// Left arrow
	else if (isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		l3PlayerX -= 5;
		l3PlayerWalking = true;
		l3FacingRight = false;

		l3WalkTimer++;

		if (l3WalkTimer >= 5)
		{
			l3WalkTimer = 0;
			l3PlayerFrame++;

			if (l3PlayerFrame > 8)
				l3PlayerFrame = 0;
		}
	}

	// No movement
	else
	{
		l3PlayerWalking = false;
		l3WalkTimer = 0;
		l3PlayerFrame = 0;   // frame[0] = idle pose (in current facing direction)
	}
}
#endif