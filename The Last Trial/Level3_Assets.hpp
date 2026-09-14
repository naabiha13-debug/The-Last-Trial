
#ifndef LEVEL3_ASSETS_HPP
#define LEVEL3_ASSETS_HPP

#include "iGraphics.h"

int waitingRoom;

int l3idleImg;
int l3walk1Img;
int l3walk2Img;
int l3walk3Img;
int l3walk4Img;

void loadLevel3Assets()
{
	waitingRoom = iLoadImage("Image//waitingroom.png");

	l3idleImg = iLoadImage("Image//player_idle.png");
	l3walk1Img = iLoadImage("Image//walk1.png");
	l3walk2Img = iLoadImage("Image//walk2.png");
	l3walk3Img = iLoadImage("Image//walk3.png");
	l3walk4Img = iLoadImage("Image//walk4.png");
}

#endif