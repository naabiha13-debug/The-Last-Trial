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
int prop1Img;
int riverBgImg[4];
const int jumpFrameCount = 8;   // CHANGED: total jump frames (frame_01 .. frame_08)
int jumpImg[jumpFrameCount];    // CHANGED: was jumpImg[6]
int rockImg;
int prop2Img;


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

	for (int i = 0; i < 4; i++)
	{
		std::string riverPath = "Image//River" + std::to_string(i + 1) + ".png";
		riverBgImg[i] = iLoadImage((char*)riverPath.c_str());
	}

	dontMoveImg = iLoadImage("Image//dontmove.png");
	paperImg = iLoadImage("Image//paper.png");
	prop1Img = iLoadImage("Image//prop1.png");
	prop2Img = iLoadImage("Image//prop2.png");

	// CHANGED: load all 8 jump frames in order (frame_01.png ... frame_08.png)
	for (int i = 0; i < jumpFrameCount; i++)
	{
		std::string num = (i + 1 < 10) ? ("0" + std::to_string(i + 1)) : std::to_string(i + 1);
		std::string jumpPath = "Image//frame_" + num + ".png";
		jumpImg[i] = iLoadImage((char*)jumpPath.c_str());
	}
	rockImg = iLoadImage("Image//rock.png");
}

#endif