#ifndef LEVEL3_JUNGLE_HPP
#define LEVEL3_JUNGLE_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include <math.h>
#include "river.hpp"

// Camera/world scroll offset
int jungleWorldX = 0;
const int jungleTotalWidth = 7000;
const int jungleScreenWidth = 1000;
const int jungleMaxScroll = jungleTotalWidth - jungleScreenWidth; // 6000

// River limits in world coordinates (river starts at screen 3)
const int riverStartWorldX = 3 * jungleScreenWidth + waveStartX; // Boat waits here
const int riverEndWorldX = 6 * jungleScreenWidth + waveEndX;     // Boat cannot go past this

// Player walking animation state
int jungleFrame = 0;
int jungleWalkTimer = 0;
bool jungleFacingRight = true;

// 3 dolls, one per background segment, static world position
int dollWorldX[3] = { 500, 1500, 2500 };
int dollY = 180;

int dollFrame[3] = { 4, 4, 4 };   // Start idle (d5)
int dollState[3] = { 0, 0, 0 };   // 0 = idle, 1 = turning to face, 2 = holding, 3 = turning back
int dollTimer[3] = { 0, 0, 0 };
int dollCyclesDone[3] = { 0, 0, 0 };
const int dollMaxCycles = 3;

int playerWorldX = 100;           // Player's actual position in the world
const int jungleMiddleX = 465;    // Screen X where the camera starts following the player
int jungleScreenPlayerY = 110;
bool jungleDead = false;
bool inBoat = false;              // True once the player has reached the river start

// Papers to collect
int paperWorldX[3] = { 650, 1650, 2650 };
int paperY = 100;
bool paperCollected[3] = { false, false, false };

// Paper pickup animation state
bool pickingUpPaper = false;
int pickupPaperFrame = 0;
int pickupPaperTimer = 0;
int activePaperIndex = -1;

// Rocks the player has to jump over
int rockWorldX[3];
const int rockWidth = 120;
const int rockHeight = 100;
const int rockY = 100; // Ground-aligned with the player's feet

// Places one rock at a random position inside each of the first three screens
void initJungleRocks()
{
	for (int i = 0; i < 3; i++)
	{
		int screenStart = i * jungleScreenWidth;
		rockWorldX[i] = screenStart + 300 + (rand() % 400); // Random spot, away from the edges
	}
}

// Jump animation state
bool jumpingJungle = false;
int jungleJumpFrame = 0;
int jungleJumpTimer = 0;

const int jumpMaxHeight = 140;
// jumpFrameCount is defined in Level3_Assets.hpp (8 frames)

// Draws the jungle and river backgrounds, the waves, the boat and the props
void drawJungleBackground()
{
	for (int i = 0; i < 7; i++)
	{
		int bgScreenX = (i * jungleScreenWidth) - jungleWorldX;

		if (i < 3)
			iShowImage(bgScreenX, 0, jungleScreenWidth, 600, jungleBgImg[i]);
		else
			iShowImage(bgScreenX, 0, jungleScreenWidth, 600, riverBgImg[i - 3]);
	}

	drawBirds(jungleWorldX, 3);
	// Waves cover the whole river (river starts at screen 3)
	drawRiverWaves(jungleWorldX, 3);
	drawUpperWaveRow(jungleWorldX, 3);

	// The boat waits at the river start, then follows the player once the player boards it
	int boatWorldX = inBoat ? playerWorldX : riverStartWorldX;
	bool boatMoving = inBoat && isSpecialKeyPressed(GLUT_KEY_RIGHT);
	drawBoat(jungleWorldX, boatWorldX, boatMoving, inBoat);

}

// Draws the prop on the seam between screen 1 and screen 2
void drawJungleProps()
{
	int propWidth = 210;   // prop1.png actual width
	int propHeight = 210;  // prop1.png actual height
	int boundaryWorldX = jungleScreenWidth; // Seam between screen 1 and screen 2

	int propScreenX = boundaryWorldX - jungleWorldX - (propWidth / 2) - 5;
	iShowImage(propScreenX, 147, propWidth, propHeight, prop1Img);
}

// Draws all rocks
void drawJungleRocks()
{
	for (int i = 0; i < 3; i++)
	{
		int screenX = rockWorldX[i] - jungleWorldX;
		iShowImage(screenX, rockY, rockWidth, rockHeight, rockImg);
	}
}

// Draws the dolls that are visible on screen
void drawJungleDolls()
{
	for (int i = 0; i < 3; i++)
	{
		int screenX = dollWorldX[i] - jungleWorldX;
		if (screenX > -100 && screenX < 1100)
			iShowImage(screenX, dollY, 80, 150, dollImg[dollFrame[i]]);
	}
}

// Draws the player (walking, jumping or picking up a paper)
void drawJunglePlayer()
{
	int screenPlayerX = playerWorldX - jungleWorldX;

	// Nothing to draw when the player is dead
	if (jungleDead)
	{
		return;
	}

	// The player is drawn as the boat while in the river
	// (remove this line if the boat images do not include the player)
	if (inBoat)
	{
		return;
	}

	// Paper pickup animation
	if (pickingUpPaper)
	{
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, pickupImg[pickupPaperFrame]);
		return;
	}

	// Jump animation: frames advance while the player rises and falls along a sine curve
	if (jumpingJungle)
	{
		float jumpProgress = jungleJumpFrame / (float)(jumpFrameCount - 1);
		int jumpOffset = (int)(sinf(jumpProgress * 3.14159f) * jumpMaxHeight);
		iShowImage(screenPlayerX, jungleScreenPlayerY + jumpOffset, 120, 150, jumpImg[jungleJumpFrame]);
		return;
	}

	// Normal walking frames
	if (jungleFacingRight)
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, l3walkFImg[jungleFrame]);
	else
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, l3walkBImg[jungleFrame]);
}

// Runs each doll's state machine: idle -> turn to face -> hold -> turn back
void updateJungleDolls()
{
	for (int i = 0; i < 3; i++)
	{
		dollTimer[i]++;

		if (dollState[i] == 0)
		{
			// Idle: wait, then start turning to face the player
			if (dollTimer[i] >= 90)
			{
				dollTimer[i] = 0;
				dollState[i] = 1;
			}
		}
		else if (dollState[i] == 1)
		{
			// Turning to face the player
			if (dollTimer[i] >= 20)
			{
				dollTimer[i] = 0;
				dollFrame[i]--;
				if (dollFrame[i] <= 0)
				{
					dollFrame[i] = 0;
					dollState[i] = 2;
				}
			}
		}
		else if (dollState[i] == 2)
		{
			// Holding: the player must not move now
			if (dollTimer[i] >= 80)
			{
				dollTimer[i] = 0;
				dollState[i] = 3;
			}
		}
		else if (dollState[i] == 3)
		{
			// Turning back to idle
			if (dollTimer[i] >= 20)
			{
				dollTimer[i] = 0;
				dollFrame[i]++;
				if (dollFrame[i] >= 4)
				{
					dollFrame[i] = 4;
					dollState[i] = 0;
					dollCyclesDone[i]++;
				}
			}
		}
	}
}

// Draws papers that are not collected yet
void drawJunglePapers()
{
	for (int i = 0; i < 3; i++)
	{
		if (!paperCollected[i] && i != activePaperIndex)
		{
			int screenX = paperWorldX[i] - jungleWorldX;
			if (screenX > -50 && screenX < 1050)
				iShowImage(screenX, paperY, 80, 80, paperImg);
		}
	}
}

// Main update function for the jungle and river level (called every tick)
void updateJungle()
{
	// Stop everything when the player is dead
	if (jungleDead)
		return;

	// Keep the wave animation running
	updateRiverWaves();

	updateBirds();

	// The player is in the boat once they reach the river start
	inBoat = (playerWorldX >= riverStartWorldX);

	// In the boat: up and down arrows move the boat vertically inside the water
	if (inBoat)
	{
		if (isSpecialKeyPressed(GLUT_KEY_UP))
			boatY += boatSpeedY;
		if (isSpecialKeyPressed(GLUT_KEY_DOWN))
			boatY -= boatSpeedY;

		if (boatY < boatMinY) boatY = boatMinY;
		if (boatY > boatMaxY) boatY = boatMaxY;
	}

	// Paper pickup animation: play the frames, then mark the paper as collected
	if (pickingUpPaper)
	{
		pickupPaperTimer++;
		if (pickupPaperTimer >= 8)
		{
			pickupPaperTimer = 0;
			pickupPaperFrame++;
			if (pickupPaperFrame > 4)
			{
				pickingUpPaper = false;
				pickupPaperFrame = 0;
				if (activePaperIndex != -1)
					paperCollected[activePaperIndex] = true;
				activePaperIndex = -1;
			}
		}
		return;
	}

	// Down arrow near a paper starts the pickup animation
	if (isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		for (int i = 0; i < 3; i++)
		{
			if (!paperCollected[i] && abs(playerWorldX - paperWorldX[i]) < 60)
			{
				pickingUpPaper = true;
				pickupPaperFrame = 0;
				pickupPaperTimer = 0;
				activePaperIndex = i;
				return;
			}
		}
	}

	// Up + right arrow starts a jump (not allowed while in the boat)
	if (!inBoat && !jumpingJungle && isSpecialKeyPressed(GLUT_KEY_UP) && isSpecialKeyPressed(GLUT_KEY_RIGHT))
		jumpingJungle = true;

	// Jump animation: advance the frames, and push the player past a rock if they land on it
	if (jumpingJungle)
	{
		jungleJumpTimer++;
		if (jungleJumpTimer >= 4)
		{
			jungleJumpTimer = 0;
			jungleJumpFrame++;
			if (jungleJumpFrame >= jumpFrameCount)
			{
				jumpingJungle = false;
				jungleJumpFrame = 0;

				for (int i = 0; i < 3; i++)
				{
					if (playerWorldX + 120 > rockWorldX[i] && playerWorldX < rockWorldX[i] + rockWidth)
						playerWorldX = rockWorldX[i] + rockWidth;
				}
			}
		}
	}

	// Moving while a doll is watching kills the player (only in the jungle part)
	bool isMoving = isSpecialKeyPressed(GLUT_KEY_RIGHT) || isSpecialKeyPressed(GLUT_KEY_LEFT);

	if (isMoving && playerWorldX < jungleScreenWidth * 3)
	{
		for (int i = 0; i < 3; i++)
		{
			if (dollState[i] == 2)
			{
				jungleDead = true;
				return;
			}
		}
	}

	const int playerWidth = 120;

	// Right edge limit: the end of the world, or the end of the river while in the boat
	int worldMax = jungleTotalWidth - playerWidth;
	if (inBoat)
		worldMax = riverEndWorldX - boatWidth;

	if (isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		jungleFacingRight = true;

		// A rock blocks the player unless they are jumping
		bool blockedByRock = false;
		if (!jumpingJungle)
		{
			for (int i = 0; i < 3; i++)
			{
				if (playerWorldX + 120 > rockWorldX[i] && playerWorldX < rockWorldX[i] + rockWidth)
					blockedByRock = true;
			}
		}

		// Move faster while jumping
		int moveSpeed = jumpingJungle ? 10 : 5;

		// Move the player forward
		if (playerWorldX < worldMax && !blockedByRock)
			playerWorldX += moveSpeed;

		// Scroll the camera once the player passes the middle of the screen
		if ((playerWorldX - jungleWorldX) > jungleMiddleX && jungleWorldX < jungleMaxScroll)
			jungleWorldX += moveSpeed;

		if (jungleWorldX > jungleMaxScroll)
			jungleWorldX = jungleMaxScroll;

		// Advance the walking animation
		jungleWalkTimer++;
		if (jungleWalkTimer >= 5)
		{
			jungleWalkTimer = 0;
			jungleFrame++;
			if (jungleFrame > 8)
				jungleFrame = 0;
		}
	}
	else if (isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		jungleFacingRight = false;

		// Move the player backward
		if (playerWorldX > 0)
			playerWorldX -= 5;

		// Scroll the camera back when the player is left of the middle
		if ((playerWorldX - jungleWorldX) < jungleMiddleX && jungleWorldX > 0)
			jungleWorldX -= 5;

		if (jungleWorldX < 0)
			jungleWorldX = 0;

		// Advance the walking animation
		jungleWalkTimer++;
		if (jungleWalkTimer >= 5)
		{
			jungleWalkTimer = 0;
			jungleFrame++;
			if (jungleFrame > 8)
				jungleFrame = 0;
		}
	}
	else
	{
		// No movement key held: reset to the idle pose
		jungleWalkTimer = 0;
		jungleFrame = 0;
	}
}

// Shows the "don't move" sign while a doll is turning or holding
void drawDontMoveSign()
{
	// No dolls in the river part
	if (playerWorldX >= jungleScreenWidth * 3)
		return;

	for (int i = 0; i < 3; i++)
	{
		if (dollState[i] == 1 || dollState[i] == 2)
		{
			iShowImage(400, 500, 200, 80, dontMoveImg);
			return;
		}
	}
}

#endif