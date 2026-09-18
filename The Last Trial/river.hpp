#ifndef RIVER_HPP
#define RIVER_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"

const int riverScreenWidth = 1000;

void drawRiverBackground(int worldX, int jungleScreenCount)
{
	for (int i = 0; i < 4; i++)
	{
		int bgScreenX = ((jungleScreenCount + i) * riverScreenWidth) - worldX;
		iShowImage(bgScreenX, 0, riverScreenWidth, 600, riverBgImg[i]);
	}
}
void drawRiverProps(int worldX, int jungleScreenCount)
{
	int propWidth = 230;
	int propHeight = 210;
	// River3/River4 seam = jungleScreenCount + 3 segments in
	int boundaryWorldX = (jungleScreenCount + 3) * riverScreenWidth;

	int propScreenX = boundaryWorldX - worldX - (propWidth / 2);
	iShowImage(propScreenX, 60, propWidth, propHeight, prop2Img);
}

#endif