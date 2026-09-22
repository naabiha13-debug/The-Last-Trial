#ifndef RIVER_HPP
#define RIVER_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"

// Width of one river background screen
const int riverScreenWidth = 1000;

// ---------------- Waves ----------------

int waveFrame = 0;
int waveTimer = 0;

const int waveTileWidth = 260;
const int waveTileHeight = 65;
const int waveRowStep = 45;
const int waveRows = 4;

const int waveStartX = 180;
const int waveEndX = 570;

// Extra rightward shift for the alternating (2nd/4th) rows
const int waveRowExtraShift = 50;

// Smaller upper wave row - only drawn over River1 and River2
const int upperWaveTileWidth = 130;   // ~50% of waveTileWidth
const int upperWaveTileHeight = 33;   // ~50% of waveTileHeight
const int upperWaveY = 190;           // Sits above the main wave rows

// ---------------- Boat ----------------

const int boatWidth = 200;
const int boatHeight = 120;
const int boatSpeedY = 5;
const int boatMinY = 20;
const int boatMaxY = 180;
const int waveDownFrame = 0;

int boatY = 100;

// Player-in-boat animation (frames 0..3, cycles while moving)
int boatPlayerFrame = 0;
int boatPlayerAnimTimer = 0;
const int boatPlayerWidth = 200;
const int boatPlayerHeight = 120;

void updateBoatPlayerAnimation(bool moving)
{
	if (moving)
	{
		boatPlayerAnimTimer++;
		if (boatPlayerAnimTimer >= 5)
		{
			boatPlayerAnimTimer = 0;
			boatPlayerFrame++;
			if (boatPlayerFrame > 3)
				boatPlayerFrame = 0;
		}
	}
	else
	{
		boatPlayerAnimTimer = 0;
		boatPlayerFrame = 0;
	}
}

const int birdCount = 5;
float birdX[birdCount], birdTurnX[birdCount];
int birdY[birdCount], birdFrame[birdCount], birdTimer[birdCount];
bool birdBack[birdCount], birdInit = false;

void updateRiverWaves()
{
	waveTimer++;
	if (waveTimer >= 10)
	{
		waveTimer = 0;
		waveFrame++;
		if (waveFrame > 2)
			waveFrame = 0;
	}
}

void drawRiverBackground(int worldX, int jungleScreenCount)
{
	for (int i = 0; i < 4; i++)
	{
		int bgScreenX = ((jungleScreenCount + i) * riverScreenWidth) - worldX;
		iShowImage(bgScreenX, 0, riverScreenWidth, 600, riverBgImg[i]);
	}
}

// Draws continuous waves over the whole river, from the bottom of the screen upward
void drawRiverWaves(int worldX, int jungleScreenCount)
{
	int zoneStart = jungleScreenCount * riverScreenWidth + waveStartX;
	int zoneEnd = (jungleScreenCount + 3) * riverScreenWidth + waveEndX;

	for (int row = waveRows - 1; row >= 0; row--)
	{
		int y = row * waveRowStep;
		int col = 0;

		for (int wx = zoneStart; wx + waveTileWidth <= zoneEnd; wx += waveTileWidth)
		{
			col++;
			int tileScreenX = wx - worldX;

			if (tileScreenX > -waveTileWidth && tileScreenX < 1000)
			{
				int frame = (waveFrame + row + col) % 3;
				iShowImage(tileScreenX, y, waveTileWidth, waveTileHeight, waveImg[frame]);
			}
		}
	}
}

// Draws a smaller extra wave row, only over River1 and River2
void drawUpperWaveRow(int worldX, int jungleScreenCount)
{
	int zoneStart = jungleScreenCount * riverScreenWidth + waveStartX; // Start where the water actually begins
	int zoneEnd = jungleScreenCount * riverScreenWidth + (3 * riverScreenWidth); // Extended through River3
	int col = 0;

	for (int wx = zoneStart; wx + upperWaveTileWidth <= zoneEnd; wx += upperWaveTileWidth)
	{
		col++;
		int tileScreenX = wx - worldX;

		if (tileScreenX > -upperWaveTileWidth && tileScreenX < 1000)
		{
			int frame = (waveFrame + col) % 3;
			iShowImage(tileScreenX, upperWaveY, upperWaveTileWidth, upperWaveTileHeight, waveImg[frame]);
		}
	}
}

// Draws the boat, and the player sprite on top only once the player has boarded
void drawBoat(int worldX, int boatWorldX, bool moving, bool showPlayer)
{
	int img = boatNormalImg;

	if (moving)
	{
		if (waveFrame == waveDownFrame)
			img = boatRightUpImg;
		else
			img = boatLeftUpImg;
	}

	iShowImage(boatWorldX - worldX, boatY, boatWidth, boatHeight, img);

	if (showPlayer)
	{
		updateBoatPlayerAnimation(moving);
		iShowImage(boatWorldX - worldX, boatY, boatPlayerWidth, boatPlayerHeight, boatPlayerImg[boatPlayerFrame]);
	}
}

void initBirds(int jungleScreenCount)
{
	if (birdInit) return;
	birdInit = true;

	int startMin = jungleScreenCount * riverScreenWidth + 40;
	int startMax = startMin + riverScreenWidth - 100;
	int turnMax = (jungleScreenCount + 3) * riverScreenWidth + riverScreenWidth / 2 + 50;

	for (int i = 0; i < birdCount; i++)
	{
		birdX[i] = startMin + rand() % (startMax - startMin);
		birdY[i] = 420 + rand() % 101;
		birdTurnX[i] = startMin + 150 + rand() % (turnMax - startMin - 150);
		birdFrame[i] = rand() % 9;
		birdBack[i] = false;
	}
}

void updateBirds()
{
	for (int i = 0; i < birdCount; i++)
	{
		if (!birdBack[i])
		{
			birdX[i] += 1.5f;
			if (birdX[i] >= birdTurnX[i]) birdBack[i] = true;
		}
		else
		{
			birdX[i] -= 1.5f;
		}

		if (++birdTimer[i] >= 6)
		{
			birdTimer[i] = 0;
			birdFrame[i] = (birdFrame[i] + 1) % 9;
		}
	}
}

void drawBirds(int worldX, int jungleScreenCount)
{
	initBirds(jungleScreenCount);
	for (int i = 0; i < birdCount; i++)
	{
		int sx = (int)birdX[i] - worldX;
		if (sx > -60 && sx < 1000)
			iShowImage(sx, birdY[i], 60, 45, birdBack[i] ? backBirdImg[birdFrame[i]] : birdImg[birdFrame[i]]);
	}
}



#endif