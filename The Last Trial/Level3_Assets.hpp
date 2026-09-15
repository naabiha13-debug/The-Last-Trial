
#ifndef LEVEL3_ASSETS_HPP
#define LEVEL3_ASSETS_HPP

#include "iGraphics.h"

int waitingRoom;

int l3idleImg;
int l3walk1Img;
int l3walk2Img;
int l3walk3Img;
int l3walk4Img;
//WaitingRoom
int cueImg;
int nextImg;
int msg1Img, msg2Img;
int take1Img, take2Img, take3Img;
int warning1Img, warning2Img;

void loadLevel3Assets()
{
	waitingRoom = iLoadImage("Image//waitingroom.png");

	l3idleImg = iLoadImage("Image//player_idle.png");
	l3walk1Img = iLoadImage("Image//walk1.png");
	l3walk2Img = iLoadImage("Image//walk2.png");
	l3walk3Img = iLoadImage("Image//walk3.png");
	l3walk4Img = iLoadImage("Image//walk4.png");

	cueImg = iLoadImage("Image//cue.png");
	nextImg = iLoadImage("Image//next.png");
	msg1Img = iLoadImage("Image//msg1.png");
	msg2Img = iLoadImage("Image//msg2.png");
	take1Img = iLoadImage("Image//take1.png");
	take2Img = iLoadImage("Image//take2.png");
	take3Img = iLoadImage("Image//take3.png");
	warning1Img = iLoadImage("Image//warning1.png");
	warning2Img = iLoadImage("Image//warning2.png");
	
}

#endif