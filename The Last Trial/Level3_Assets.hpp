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
int propImg[6];
int riverBgImg[4];
int waveImg[3];
const int jumpFrameCount = 8;
int jumpImg[jumpFrameCount];
int rockImg;
int boatNormalImg, boatRightUpImg, boatLeftUpImg;
int boatPlayerImg[4];
int birdImg[9];
int backBirdImg[9];
int crGunAttackImg[2];

// River 4 landing sequence images
int msg3Img;   // shown right after the player lands on the River 4 bank (was pinmsgImg / pinmsg.png)
int msg4Img;   // shown after the player clicks the control room door
int pinImg;    // final pin image, shown at the end of the click sequence

int mImg;
int mergeImg;
int treePropImg[2];
int paperPickupImg[5];   // <-- merged in from teammate's second version

int controlRoomBgImg[3];
int controlRoomMonitorImg;
int gunImg;
int crAttackImg[6];
int guardWalkImg[4];
int guardAttackImg[5];
int bottleImg;
int treeImg;
int crHealthImg[4];   
int uniformMsgImg;          // "press X to exchange uniform" message
int crUniformWalkImg[8];    // new player sprite after uniform swap
int gunMsgImg;   // "press C to collect gun" message
int crGunWalkImg[6];
int crMaskWalkImg[5];
int crMaskGunAttackImg[2];
int usbCollectedImg;
int usbImg;
int usbSuccessImg;

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

	// merged: paper pickup animation frames
	for (int i = 0; i < 5; i++)
	{
		std::string paperPickupPath = "Image//pickPaper-" + std::to_string(i + 1) + ".png";
		paperPickupImg[i] = iLoadImage((char*)paperPickupPath.c_str());
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
	for (int i = 0; i < 6; i++)
	{
		std::string propPath = "Image//prop" + std::to_string(i + 1) + ".png";
		propImg[i] = iLoadImage((char*)propPath.c_str());
	}

	for (int i = 0; i < 2; i++)
	{
		std::string treePropPath = "Image//treeprop" + std::to_string(i + 1) + ".png";
		treePropImg[i] = iLoadImage((char*)treePropPath.c_str());
	}
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
	bottleImg=iLoadImage( "Image//Bottol.png");
	treeImg = iLoadImage("Image//tree.png");
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

	// River 4 landing sequence
	msg3Img = iLoadImage("Image//msg3.png");
	msg4Img = iLoadImage("Image//msg4.png");
	pinImg = iLoadImage("Image//pin.png");

	mImg = iLoadImage("Image//m.png");
	mergeImg = iLoadImage("Image//merge.png");

	for (int i = 0; i < 3; i++)
	{
		std::string path = "Image//c" + std::to_string(i + 1) + ".png";
		controlRoomBgImg[i] = iLoadImage((char*)path.c_str());
	}
	controlRoomMonitorImg = iLoadImage("Image//monitor.png");
	gunImg = iLoadImage("Image//gun.png");

	for (int i = 0; i < 6; i++)
	{
		std::string path = "Image//attack" + std::to_string(i + 1) + ".png";
		crAttackImg[i] = iLoadImage((char*)path.c_str());
	}

	for (int i = 0; i < 4; i++)
	{
		std::string path = "Image//Guardwalk" + std::to_string(i + 1) + ".png";
		guardWalkImg[i] = iLoadImage((char*)path.c_str());
	}
	for (int i = 0; i < 5; i++)
	{
		std::string path = "Image//GuardAttack" + std::to_string(i + 1) + ".png";
		guardAttackImg[i] = iLoadImage((char*)path.c_str());
	}
	for (int i = 0; i < 4; i++)
	{
		std::string path = "Image//health" + std::to_string(i + 1) + ".png";
		crHealthImg[i] = iLoadImage((char*)path.c_str());
	}
	uniformMsgImg = iLoadImage("Image//uniformmsg.png");

	for (int i = 0; i < 8; i++)
	{
		std::string path = "Image//PWalk" + std::to_string(i + 1) + ".png";
		crUniformWalkImg[i] = iLoadImage((char*)path.c_str());
	}
	gunMsgImg = iLoadImage("Image//gunmsg.png");
	for (int i = 0; i < 6; i++)
	{
		std::string path = "Image//GWalk" + std::to_string(i + 1) + ".png";
		crGunWalkImg[i] = iLoadImage((char*)path.c_str());
	}
	for (int i = 0; i < 2; i++)
	{
		std::string path = "Image//attackgun" + std::to_string(i + 1) + ".png";
		crGunAttackImg[i] = iLoadImage((char*)path.c_str());
	}
	for (int i = 0; i < 5; i++)
	{
		std::string path = "Image//MaskWalk" + std::to_string(i + 1) + ".png";
		crMaskWalkImg[i] = iLoadImage((char*)path.c_str());
	}

	for (int i = 0; i < 2; i++)
	{
		std::string path = "Image//MaskGunAttack" + std::to_string(i + 1) + ".png";
		crMaskGunAttackImg[i] = iLoadImage((char*)path.c_str());
	}

	usbCollectedImg = iLoadImage("Image//usbcollected.png");
	usbImg = iLoadImage("Image//u.png");
	usbSuccessImg = iLoadImage("Image//Successful.png");
}

#endif