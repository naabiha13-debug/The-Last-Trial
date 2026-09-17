#ifndef LEVEL3_ASSETS_HPP
#define LEVEL3_ASSETS_HPP

#include "iGraphics.h"
#include <string>

int waitingRoom;

// Forward (right) walk frames - frame[0] also doubles as idle pose
int l3walkFImg[9];
// Backward (left) walk frames
int l3walkBImg[9];

//WaitingRoom
int cueImg;
int nextImg;
int msg1Img, msg2Img;
int pickupImg[5];
int warning1Img, warning2Img;
int jungleBgImg[3];
int dollImg[5];
int dontMoveImg;
int paperImg;

void loadLevel3Assets()
{
	waitingRoom = iLoadImage("Image//waitingroom.png");

	for (int i = 0; i < 9; i++)
	{
		std::string num = (i + 1 < 10) ? ("0" + std::to_string(i + 1)) : std::to_string(i + 1);

		std::string forwardPath = "Image//walk_forward_frame_" + num + ".png";
		l3walkFImg[i] = iLoadImage((char*)forwardPath.c_str());

		std::string backwardPath = "Image//walk_backward_frame_" + num + ".png";
		l3walkBImg[i] = iLoadImage((char*)backwardPath.c_str());
	}

	cueImg = iLoadImage("Image//cue.png");
	nextImg = iLoadImage("Image//next.png");
	msg1Img = iLoadImage("Image//msg1.png");
	msg2Img = iLoadImage("Image//msg2.png");
	for (int i = 0; i < 5; i++)
	{
		std::string pickupPath = "Image//pickup-" + std::to_string(i + 1) + ".png";
		pickupImg[i] = iLoadImage((char*)pickupPath.c_str());
	}
	warning1Img = iLoadImage("Image//warning1.png");
	warning2Img = iLoadImage("Image//warning2.png");
	for (int i = 0; i < 3; i++)
	{
		std::string junglePath = "Image//jungle" + std::to_string(i + 1) + ".png";
		jungleBgImg[i] = iLoadImage((char*)junglePath.c_str());
	}
	for (int i = 0; i < 5; i++)
	{
		std::string dollPath = "Image//d" + std::to_string(i + 1) + ".png";
		dollImg[i] = iLoadImage((char*)dollPath.c_str());
	}
	dontMoveImg = iLoadImage("Image//dontmove.png");
	paperImg = iLoadImage("Image//paper.png");
}

#endif