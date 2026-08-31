#ifndef LEVEL2_ASSET_HPP
#define LEVEL2_ASSET_HPP
#include "iGraphics.h"

int playerBg[5];
int botBg[5];

void loadLevel2Assets()
{
	playerBg[0] = iLoadImage("Image//bg1.png");
	playerBg[1] = iLoadImage("Image//bg2.png");
	playerBg[2] = iLoadImage("Image//bg3.png");
	playerBg[3] = iLoadImage("Image//bg4.png");
	playerBg[4] = iLoadImage("Image//bg5.png");

	botBg[0] = iLoadImage("Image//b1.png");
	botBg[1] = iLoadImage("Image//b2.png");
	botBg[2] = iLoadImage("Image//b3.png");
	botBg[3] = iLoadImage("Image//b4.png");
	botBg[4] = iLoadImage("Image//b5.png");
}
#endif