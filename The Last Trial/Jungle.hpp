#ifndef LEVEL3_JUNGLE_HPP
#define LEVEL3_JUNGLE_HPP

#include "iGraphics.h"
#include "Level3_Assets.hpp"
#include <math.h>
#include "river.hpp"

// Camera/world scroll offset
int jungleWorldX = 0;
const int jungleTotalWidth = 7000;
const int jungleScreenWidth = 1000;
const int jungleMaxScroll = jungleTotalWidth - jungleScreenWidth;

// River limits in world coordinates (river starts at screen 3)
const int riverStartWorldX = 3 * jungleScreenWidth + waveStartX;
const int riverEndWorldX = 6 * jungleScreenWidth + waveEndX;

// Player walking animation state
int jungleFrame = 0;
int jungleWalkTimer = 0;
bool jungleFacingRight = true;

// 3 dolls, one per background segment, static world position
int dollWorldX[3] = { 500, 1500, 2500 };
int dollY = 180;

int dollFrame[3] = { 4, 4, 4 };
int dollState[3] = { 0, 0, 0 };
int dollTimer[3] = { 0, 0, 0 };
int dollCyclesDone[3] = { 0, 0, 0 };
const int dollMaxCycles = 3;

int playerWorldX = 100;
const int jungleMiddleX = 465;

// Normal walking Y. On River 4's bank this is raised to river4BankPlayerY.
int jungleScreenPlayerY = 110;

// Y used only after landing on the River 4 bank, since that art sits higher than the rest.
const int river4BankPlayerY = 150;   // TODO: adjust to match the art

// Y position the boat sits at once it has docked at the river's end.
const int boatDockedY = 100;

bool jungleDead = false;
bool exitingBoat = false;      // true while the jump-off-the-boat animation is playing
bool reachedRiverEnd = false;
// NOTE: inBoat and boardingBoat are declared in river.hpp — NOT redeclared here

// ---------------- River 4 landing / control room sequence ----------------
bool showMsg3 = false;
bool showMsg4 = false;

bool waitingForMergeClick = false;
bool waitingForPinClick = false;
bool showPinFinal = false;

// Defined in the pin keypad file, included after this one.
void handlePinKeypadClick(int mx, int my);
void drawPinDisplay();

// Screen box for the control room door.
// TODO: fine-tune these four numbers by testing in-game.
const int controlRoomBtnX = 850;
const int controlRoomBtnY = 198;
const int controlRoomBtnW = 140;
const int controlRoomBtnH = 168;

// Papers to collect
int paperWorldX[3] = { 650, 1650, 2750 };
int paperY = 100;
bool paperCollected[3] = { false, false, false };

// Paper pickup animation state
bool pickingUpPaper = false;
int pickupPaperFrame = 0;
int pickupPaperTimer = 0;
int activePaperIndex = -1;

// Rocks the player has to jump over
int rockWorldX[3];
const int rockWidth = 120;
const int rockHeight = 100;
const int rockY = 100;

void initJungleRocks()
{
	for (int i = 0; i < 3; i++)
	{
		int screenStart = i * jungleScreenWidth;
		rockWorldX[i] = screenStart + 300 + (rand() % 400);
	}
}

void drawJungleProps(bool frontPass);   // defined below

// Jump animation state
bool jumpingJungle = false;
int jungleJumpFrame = 0;
int jungleJumpTimer = 0;
const int jumpMaxHeight = 140;

void drawJungleBackground()
{
	for (int i = 0; i < 7; i++)
	{
		int bgScreenX = (i * jungleScreenWidth) - jungleWorldX;

		if (i < 3)
			iShowImage(bgScreenX, 0, jungleScreenWidth, 600, jungleBgImg[i]);
		else
			iShowImage(bgScreenX, 0, jungleScreenWidth, 600, riverBgImg[i - 3]);
	}

	drawBirds(jungleWorldX, 3);
	drawRiverWaves(jungleWorldX, 3);
	drawUpperWaveRow(jungleWorldX, 3);

	drawJungleProps(false);   // props behind the boat

	bool boatVisible = inBoat || reachedRiverEnd || boardingBoat;
	if (boatVisible)
	{
		int boatWorldX = inBoat ? playerWorldX : (reachedRiverEnd ? (riverEndWorldX - boatWidth) : riverStartWorldX);
		bool boatMoving = inBoat && isSpecialKeyPressed(GLUT_KEY_RIGHT);
		bool showBoatPlayer = inBoat || boardingBoat;   // show player on the boat while boarding too
		drawBoat(jungleWorldX, boatWorldX, boatMoving, showBoatPlayer);
	}

	drawJungleProps(true);    // props in front of the boat
}

// frontPass = false draws props behind the boat, true draws props in front of it
void drawJungleProps(bool frontPass)
{
	int propWidth = 210;
	int propHeight = 210;
	int boundaryWorldX = jungleScreenWidth;

	int propScreenX = boundaryWorldX - jungleWorldX - (propWidth / 2) - 5;
	int propY = 147;
	if ((propY <= boatY) == frontPass)
		iShowImage(propScreenX, propY, propWidth, propHeight, propImg[0]);

	int treePropY = 147 + 300;
	if ((treePropY <= boatY) == frontPass)
		iShowImage(propScreenX, treePropY, propWidth, propHeight, treePropImg[0]);

	int riverBoundaryWorldX = 3 * jungleScreenWidth;
	int riverPropScreenX = riverBoundaryWorldX - jungleWorldX - (propWidth / 2) - 5;
	if ((treePropY <= boatY) == frontPass)
		iShowImage(riverPropScreenX - 20, treePropY, propWidth, propHeight, treePropImg[0]);

	int prop3Y = treePropY - propHeight - 120;
	if ((prop3Y <= boatY) == frontPass)
		iShowImage(riverPropScreenX, prop3Y, propWidth, propHeight - 10, propImg[2]);

	int river1PropWorldX = riverBoundaryWorldX + 300;
	int river1PropScreenX = river1PropWorldX - jungleWorldX;

	int prop4Y = boatY + 70;
	if ((prop4Y <= boatY) == frontPass)
		iShowImage(river1PropScreenX - 75, prop4Y, propWidth, propHeight - 20, propImg[3]);

	int river3to4BoundaryWorldX = 6 * jungleScreenWidth;
	int river3to4PropScreenX = river3to4BoundaryWorldX - jungleWorldX - (propWidth / 2) - 5;
	if ((152 <= boatY) == frontPass)
		iShowImage(river3to4PropScreenX - 20, 152, propWidth + 10, propHeight, propImg[1]);

	int prop5Y = boatY - 120;
	if ((prop5Y <= boatY) == frontPass)
		iShowImage(river1PropScreenX - 80, prop5Y, propWidth + 10, propHeight + 10, propImg[0]);
}

void drawJungleRocks()
{
	for (int i = 0; i < 3; i++)
	{
		int screenX = rockWorldX[i] - jungleWorldX;
		iShowImage(screenX, rockY, rockWidth, rockHeight, rockImg);
	}
}

void drawJungleDolls()
{
	for (int i = 0; i < 3; i++)
	{
		int screenX = dollWorldX[i] - jungleWorldX;
		if (screenX > -100 && screenX < 1100)
			iShowImage(screenX, dollY, 80, 150, dollImg[dollFrame[i]]);
	}
}

void drawJunglePlayer()
{
	int screenPlayerX = playerWorldX - jungleWorldX;

	if (jungleDead)
		return;

	if (inBoat || boardingBoat)   // boat already shows the player while boarding
		return;

	if (pickingUpPaper)
	{
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, pickupImg[pickupPaperFrame]);
		return;
	}

	if (jumpingJungle)
	{
		float jumpProgress = jungleJumpFrame / (float)(jumpFrameCount - 1);
		int jumpOffset = (int)(sinf(jumpProgress * 3.14159f) * jumpMaxHeight);
		iShowImage(screenPlayerX, jungleScreenPlayerY + jumpOffset, 120, 150, jumpImg[jungleJumpFrame]);
		return;
	}

	if (jungleFacingRight)
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, l3walkFImg[jungleFrame]);
	else
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, l3walkBImg[jungleFrame]);
}

void updateJungleDolls()
{
	for (int i = 0; i < 3; i++)
	{
		dollTimer[i]++;

		if (dollState[i] == 0)
		{
			if (dollTimer[i] >= 90)
			{
				dollTimer[i] = 0;
				dollState[i] = 1;
			}
		}
		else if (dollState[i] == 1)
		{
			if (dollTimer[i] >= 20)
			{
				dollTimer[i] = 0;
				dollFrame[i]--;
				if (dollFrame[i] <= 0)
				{
					dollFrame[i] = 0;
					dollState[i] = 2;
				}
			}
		}
		else if (dollState[i] == 2)
		{
			if (dollTimer[i] >= 80)
			{
				dollTimer[i] = 0;
				dollState[i] = 3;
			}
		}
		else if (dollState[i] == 3)
		{
			if (dollTimer[i] >= 20)
			{
				dollTimer[i] = 0;
				dollFrame[i]++;
				if (dollFrame[i] >= 4)
				{
					dollFrame[i] = 4;
					dollState[i] = 0;
					dollCyclesDone[i]++;
				}
			}
		}
	}
}

void drawJunglePapers()
{
	for (int i = 0; i < 3; i++)
	{
		if (!paperCollected[i] && i != activePaperIndex)
		{
			int screenX = paperWorldX[i] - jungleWorldX;
			if (screenX > -50 && screenX < 1050)
				iShowImage(screenX, paperY, 80, 80, paperImg);
		}
	}
}

void drawMsg3()
{
	if (!showMsg3)
		return;

	int screenPlayerX = playerWorldX - jungleWorldX;

	int msgWidth = 320;
	int msgHeight = 230;
	int msgX = screenPlayerX + (120 - msgWidth) / 2;
	int msgY = jungleScreenPlayerY + 100;

	iShowImage(msgX, msgY, msgWidth, msgHeight, msg3Img);
}

void drawMsg4()
{
	if (!showMsg4)
		return;

	iShowImage(340, 200, 320, 230, msg4Img);   // TODO: position/size to match art
}

bool showMImage = false;
int mImageTimer = 0;
const int mImageDuration = 120;
bool showMergeImage = false;

void drawMImage()
{
	if (!showMImage)
		return;

	iShowImage(400, 350, 450, 20, mImg);
}

void drawMergeImage()
{
	if (!showMergeImage)
		return;

	iShowImage(0, 0, 1000, 600, mergeImg);
}

void drawPinFinal()
{
	if (!showPinFinal)
		return;

	iShowImage(0, 0, 1000, 600, pinImg);
}

void handleJungleClick(int mx, int my)
{
	if (showPinFinal)
	{
		handlePinKeypadClick(mx, my);
		return;
	}

	if (showMsg3)
	{
		bool insideControlRoom =
			mx >= controlRoomBtnX && mx <= controlRoomBtnX + controlRoomBtnW &&
			my >= controlRoomBtnY && my <= controlRoomBtnY + controlRoomBtnH;

		if (insideControlRoom)
		{
			showMsg3 = false;
			showMsg4 = true;
		}
		return;
	}

	if (showMsg4)
	{
		showMsg4 = false;
		showMImage = true;
		mImageTimer = 0;
		return;
	}

	if (waitingForMergeClick)
	{
		waitingForMergeClick = false;
		showMergeImage = true;
		waitingForPinClick = true;
		return;
	}

	if (waitingForPinClick)
	{
		waitingForPinClick = false;
		showMergeImage = false;
		showPinFinal = true;
	}
}

void updateJungle()
{
	if (showMImage)
	{
		mImageTimer++;
		if (mImageTimer >= mImageDuration)
		{
			showMImage = false;
			mImageTimer = 0;
			waitingForMergeClick = true;
		}
	}

	if (jungleDead)
		return;

	updateRiverWaves();
	updateBirds();

	if (inBoat)
	{
		if (isSpecialKeyPressed(GLUT_KEY_UP))
			boatY += boatSpeedY;
		if (isSpecialKeyPressed(GLUT_KEY_DOWN))
			boatY -= boatSpeedY;

		if (boatY < boatMinY) boatY = boatMinY;
		if (boatY > boatMaxY) boatY = boatMaxY;
	}

	if (inBoat && !jumpingJungle && isSpecialKeyPressed(GLUT_KEY_UP) && isSpecialKeyPressed(GLUT_KEY_RIGHT)
		&& playerWorldX >= (riverEndWorldX - boatWidth))
	{
		jumpingJungle = true;
		exitingBoat = true;
		inBoat = false;
		reachedRiverEnd = true;
		boatY = boatDockedY;
	}

	if (pickingUpPaper)
	{
		pickupPaperTimer++;
		if (pickupPaperTimer >= 8)
		{
			pickupPaperTimer = 0;
			pickupPaperFrame++;
			if (pickupPaperFrame > 4)
			{
				pickingUpPaper = false;
				pickupPaperFrame = 0;
				if (activePaperIndex != -1)
					paperCollected[activePaperIndex] = true;
				activePaperIndex = -1;
			}
		}
		return;
	}

	if (isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		for (int i = 0; i < 3; i++)
		{
			if (!paperCollected[i] && abs(playerWorldX - paperWorldX[i]) < 60)
			{
				pickingUpPaper = true;
				pickupPaperFrame = 0;
				pickupPaperTimer = 0;
				activePaperIndex = i;
				return;
			}
		}
	}

	if (!inBoat && !jumpingJungle && isSpecialKeyPressed(GLUT_KEY_UP) && isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		jumpingJungle = true;
		if (playerWorldX + 120 >= riverStartWorldX - 30)
			boardingBoat = true;
	}

	if (jumpingJungle)
	{
		jungleJumpTimer++;
		if (jungleJumpTimer >= 4)
		{
			jungleJumpTimer = 0;
			jungleJumpFrame++;
			if (jungleJumpFrame >= jumpFrameCount)
			{
				jumpingJungle = false;
				jungleJumpFrame = 0;

				for (int i = 0; i < 3; i++)
				{
					if (playerWorldX + 120 > rockWorldX[i] && playerWorldX < rockWorldX[i] + rockWidth)
						playerWorldX = rockWorldX[i] + rockWidth;
				}

				if (boardingBoat)
				{
					boardingBoat = false;
					inBoat = true;
					playerWorldX = riverStartWorldX;
				}

				if (exitingBoat)
				{
					exitingBoat = false;
					playerWorldX = riverEndWorldX;
					jungleFacingRight = true;

					jungleScreenPlayerY = river4BankPlayerY;
					showMsg3 = true;
				}
			}
		}
	}

	bool isMoving = isSpecialKeyPressed(GLUT_KEY_RIGHT) || isSpecialKeyPressed(GLUT_KEY_LEFT);

	if (isMoving && !inBoat && !reachedRiverEnd && playerWorldX < jungleScreenWidth * 3)
	{
		for (int i = 0; i < 3; i++)
		{
			if (dollState[i] == 2)
			{
				jungleDead = true;
				return;
			}
		}
	}

	const int playerWidth = 120;

	int worldMax = jungleTotalWidth - playerWidth;
	if (inBoat)
		worldMax = riverEndWorldX - boatWidth;
	else if (!jumpingJungle && !reachedRiverEnd)
		worldMax = riverStartWorldX - playerWidth;

	if (isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		jungleFacingRight = true;

		bool blockedByRock = false;
		if (!jumpingJungle)
		{
			for (int i = 0; i < 3; i++)
			{
				if (playerWorldX + 120 > rockWorldX[i] && playerWorldX < rockWorldX[i] + rockWidth)
					blockedByRock = true;
			}
		}

		int moveSpeed = jumpingJungle ? 10 : 5;

		if (playerWorldX < worldMax && !blockedByRock)
			playerWorldX += moveSpeed;

		if ((playerWorldX - jungleWorldX) > jungleMiddleX && jungleWorldX < jungleMaxScroll)
			jungleWorldX += moveSpeed;

		if (jungleWorldX > jungleMaxScroll)
			jungleWorldX = jungleMaxScroll;

		jungleWalkTimer++;
		if (jungleWalkTimer >= 5)
		{
			jungleWalkTimer = 0;
			jungleFrame++;
			if (jungleFrame > 8)
				jungleFrame = 0;
		}
	}
	else if (isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		jungleFacingRight = false;

		if (playerWorldX > 0)
			playerWorldX -= 5;

		if ((playerWorldX - jungleWorldX) < jungleMiddleX && jungleWorldX > 0)
			jungleWorldX -= 5;

		if (jungleWorldX < 0)
			jungleWorldX = 0;

		jungleWalkTimer++;
		if (jungleWalkTimer >= 5)
		{
			jungleWalkTimer = 0;
			jungleFrame++;
			if (jungleFrame > 8)
				jungleFrame = 0;
		}
	}
	else
	{
		jungleWalkTimer = 0;
		jungleFrame = 0;
	}
}

void drawDontMoveSign()
{
	if (reachedRiverEnd || playerWorldX >= jungleScreenWidth * 3)
		return;

	for (int i = 0; i < 3; i++)
	{
		if (dollState[i] == 1 || dollState[i] == 2)
		{
			iShowImage(400, 500, 200, 80, dontMoveImg);
			return;
		}
	}
}

const int totalControlRoomDistance = 600;

void drawDistanceCounter()
{
	float progress = (float)playerWorldX / (float)riverEndWorldX;
	if (progress < 0.0f) progress = 0.0f;
	if (progress > 1.0f) progress = 1.0f;

	int remaining = totalControlRoomDistance - (int)(progress * totalControlRoomDistance);
	if (remaining < 0) remaining = 0;

	char buffer[64];
	sprintf_s(buffer, sizeof(buffer), "CONTROL ROOM DISTANCE : %dm", remaining);

	iSetColor(255, 255, 255);
	iText(650, 560, buffer, GLUT_BITMAP_HELVETICA_18);
}

#endif