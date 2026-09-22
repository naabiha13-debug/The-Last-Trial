#ifndef LEVEL3_ASSETS_HPP
#define LEVEL3_ASSETS_HPP

#include "iGraphics.h"
#include <string>

int waitingRoom;

// Forward (right) walk frames - frame[0] also doubles as idle pose
int l3walkFImg[9];
// Backward (left) walk frames
int l3walkBImg[9];

// Teammate's additions
int exploreImg;
int lampImg[8];
int l3IntroImg[3];
int keyMsgImg;
int keyFitsImg;

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
int waveImg[3];
const int jumpFrameCount = 8;
int jumpImg[jumpFrameCount];
int rockImg;
int prop2Img;
int boatNormalImg, boatRightUpImg, boatLeftUpImg;
int boatPlayerImg[4];
int birdImg[9];
int backBirdImg[9];


void loadLevel3Assets()
{
	exploreImg = iLoadImage("Image//explore.png");
	keyMsgImg = iLoadImage("Image//key.png");
	keyFitsImg = iLoadImage("Image//keyFits.png");
	for (int i = 0; i < 3; i++)
	{
		std::string introPath = "Image//l3g" + std::to_string(i + 1) + ".png";
		l3IntroImg[i] = iLoadImage((char*)introPath.c_str());
	}
	waitingRoom = iLoadImage("Image//waitingroom.png");
	for (int i = 0; i < 8; i++)
	{
		std::string lampPath = "Image//light" + std::to_string(i + 1) + ".png";
		lampImg[i] = iLoadImage((char*)lampPath.c_str());
	}

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

	for (int i = 0; i < 3; i++)
	{
		std::string wavePath = "Image//wave_strip_" + std::to_string(i + 1) + ".png";
		waveImg[i] = iLoadImage((char*)wavePath.c_str());
	}

	for (int i = 0; i < jumpFrameCount; i++)
	{
		std::string num = (i + 1 < 10) ? ("0" + std::to_string(i + 1)) : std::to_string(i + 1);
		std::string jumpPath = "Image//frame_" + num + ".png";
		jumpImg[i] = iLoadImage((char*)jumpPath.c_str());
	}
	rockImg = iLoadImage("Image//rock.png");

	boatNormalImg = iLoadImage("Image//boat_normal.png");
	boatRightUpImg = iLoadImage("Image//boat_right_up.png");
	boatLeftUpImg = iLoadImage("Image//boat_left_up.png");

	for (int i = 0; i < 4; i++)
	{
		std::string boatPlayerPath = "Image//boat" + std::to_string(i + 1) + ".png";
		boatPlayerImg[i] = iLoadImage((char*)boatPlayerPath.c_str());
	}

	for (int i = 0; i < 9; i++)
	{
		birdImg[i] = iLoadImage((char*)("Image//bird" + std::to_string(i + 1) + ".png").c_str());
		backBirdImg[i] = iLoadImage((char*)("Image//bb" + std::to_string(i + 1) + ".png").c_str());
	}
}

#endif