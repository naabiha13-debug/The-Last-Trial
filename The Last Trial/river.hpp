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

const int waveRowExtraShift = 50;

const int upperWaveTileWidth = 130;
const int upperWaveTileHeight = 33;
const int upperWaveY = 190;

// ---------------- Boat ----------------

const int boatWidth = 200;
const int boatHeight = 120;
const int boatSpeedY = 5;
const int boatMinY = 20;
const int boatMaxY = 180;
const int waveDownFrame = 0;

int boatY = 100;
bool inBoat = false;
bool boardingBoat = false;

int boatPlayerFrame = 0;
int boatPlayerAnimTimer = 0;
const int boatPlayerWidth = 120;
const int boatPlayerHeight = 170;
int boatPlayerOffsetX = 20;
int boatPlayerOffsetY = 0;

void updateBoatPlayerAnimation(bool moving)
{
	if (moving)
	{
		boatPlayerAnimTimer++;
		if (boatPlayerAnimTimer >= 60)
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
float birdX[birdCount];
float birdSpeed[birdCount];
int birdY[birdCount], birdFrame[birdCount], birdTimer[birdCount];
bool birdBack[birdCount], birdInit = false;

int birdDir[birdCount];
int birdZoneStart = 0, birdZoneEnd = 0;

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

void drawUpperWaveRow(int worldX, int jungleScreenCount)
{
	int zoneStart = jungleScreenCount * riverScreenWidth + waveStartX;
	int zoneEnd = jungleScreenCount * riverScreenWidth + (3 * riverScreenWidth);
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
		int playerScreenX = boatWorldX - worldX + boatPlayerOffsetX;
		int playerScreenY = boatY + boatPlayerOffsetY;
		iShowImage(playerScreenX, playerScreenY, boatPlayerWidth, boatPlayerHeight, boatPlayerImg[boatPlayerFrame]);
	}
}

void initBirds(int jungleScreenCount)
{
	if (birdInit) return;
	birdInit = true;

	birdZoneStart = jungleScreenCount * riverScreenWidth + 40;
	birdZoneEnd = (jungleScreenCount + 3) * riverScreenWidth + riverScreenWidth / 2 + 50;

	for (int i = 0; i < birdCount; i++)
	{
		birdDir[i] = (i % 2 == 0) ? 1 : -1;
		birdX[i] = (float)(birdZoneStart + rand() % (birdZoneEnd - birdZoneStart));
		birdY[i] = 380 + rand() % 150;
		birdFrame[i] = rand() % 9;
		birdBack[i] = (birdDir[i] == -1);
		birdSpeed[i] = 1.0f + (rand() % 100) / 100.0f;
	}
}

void updateBirds()
{
	for (int i = 0; i < birdCount; i++)
	{
		birdX[i] += birdDir[i] * birdSpeed[i];

		if (birdDir[i] == 1 && birdX[i] > birdZoneEnd)
		{
			birdX[i] = (float)birdZoneEnd;
			birdDir[i] = -1;
			birdBack[i] = true;
		}
		else if (birdDir[i] == -1 && birdX[i] < birdZoneStart)
		{
			birdX[i] = (float)birdZoneStart;
			birdDir[i] = 1;
			birdBack[i] = false;
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