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
int hintsCollected = 0;

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
		rockWorldX[i] = screenStart + 200 + (rand() % 300);   // shifted further left
	}
}

// ---------------- Bottle (extra hint pickups, scattered across the river) ----------------
// NOTE: "bottleImg" must be declared and loaded in Level3_Assets.hpp
// (e.g. `int bottleImg;` loaded via iLoadImage/iLoadPNG pointing to "Bottol.png")
// before this will actually draw anything.

const int bottleCount = 3;
int bottleWorldX[bottleCount];
int bottleY[bottleCount];
bool bottleCollected[bottleCount] = { false, false, false };
bool bottleInit = false;

void initBottles()
{
	if (bottleInit) return;
	bottleInit = true;

	int zoneStart = riverStartWorldX;
	int zoneSpan = riverEndWorldX - riverStartWorldX - 80;

	for (int i = 0; i < bottleCount; i++)
	{
		int candidateX;
		bool tooCloseToTree;
		do
		{
			candidateX = zoneStart + (rand() % zoneSpan);
			tooCloseToTree = false;
			for (int j = 0; j < treeCount; j++)
			{
				if (abs(candidateX - treeWorldX[j]) < 100)
				{
					tooCloseToTree = true;
					break;
				}
			}
		} while (tooCloseToTree);

		bottleWorldX[i] = candidateX;
		bottleY[i] = boatMinY + (rand() % (boatMaxY - boatMinY));
	}
}

                                                                   

void drawJungleProps(bool frontPass);   // defined below

// Jump animation state
bool jumpingJungle = false;
int jungleJumpFrame = 0;
int jungleJumpTimer = 0;
const int jumpMaxHeight = 140;

// frontPass = false draws bottles behind the boat, true draws bottles in front of it
void drawBottles(bool frontPass)
{
	for (int i = 0; i < bottleCount; i++)
	{
		if (bottleCollected[i])
			continue;

		// If bottle's Y is <= boat's Y (bottle is "above"/behind in stacking),
		// draw it in the frontPass == false pass (behind boat).
		// Otherwise draw it in the frontPass == true pass (in front of / above boat).
		if ((bottleY[i] <= boatY) == frontPass)
		{
			int screenX = bottleWorldX[i] - jungleWorldX;
			if (screenX > -60 && screenX < 1060)
				iShowImage(screenX, bottleY[i], 60, 60, bottleImg);
		}
	}
}

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
	initTrees(3);
	initBottles();
	initAxe(3);

	drawBirds(jungleWorldX, 3);
	drawRiverWaves(jungleWorldX, 3);
	drawUpperWaveRow(jungleWorldX, 3);

	drawJungleProps(false);   // props behind the boat
	drawBottles(false);       // bottles that sit "behind" the boat's current Y
	drawAxe(jungleWorldX, false);
	drawTrees(jungleWorldX, false);

	bool nearRiverStart = !inBoat && !boardingBoat && !reachedRiverEnd &&
		(riverStartWorldX - jungleWorldX) > -boatWidth &&
		(riverStartWorldX - jungleWorldX) < jungleScreenWidth;

	bool boatVisible = inBoat || reachedRiverEnd || boardingBoat || nearRiverStart;
	if (boatVisible)
	{
		int boatWorldX = inBoat ? playerWorldX : (reachedRiverEnd ? (riverEndWorldX - boatWidth) : riverStartWorldX);
		bool boatMoving = inBoat && isSpecialKeyPressed(GLUT_KEY_RIGHT);
		bool showBoatPlayer = inBoat;  // show player on the boat while boarding too
		drawBoat(jungleWorldX, boatWorldX, boatMoving, showBoatPlayer);
	}

	drawJungleProps(true);    // props in front of the boat
	drawBottles(true);        // bottles that sit "in front of" the boat's current Y
	drawAxe(jungleWorldX, true);
	drawTrees(jungleWorldX, true);
}

// frontPass = false draws props behind the boat, true draws props in front of it
void drawJungleProps(bool frontPass)
{
	int propWidth = 210;
	int propHeight = 210;
	int boundaryWorldX = jungleScreenWidth;

	// Fixed baseline for prop positions 
	const int boatRestY = 100;

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

	// prop position fixed 
	int prop4Y = boatRestY + 70;
	if ((prop4Y <= boatY) == frontPass)
		iShowImage(river1PropScreenX - 75, prop4Y, propWidth, propHeight - 20, propImg[3]);

	int river3to4BoundaryWorldX = 6 * jungleScreenWidth;
	int river3to4PropScreenX = river3to4BoundaryWorldX - jungleWorldX - (propWidth / 2) - 5;
	if ((152 <= boatY) == frontPass)
		iShowImage(river3to4PropScreenX - 20, 152, propWidth + 10, propHeight, propImg[1]);

	int prop5Y = boatRestY - 120;
	if ((prop5Y <= boatY) == frontPass)
		iShowImage(river1PropScreenX - 80, prop5Y, propWidth + 10, propHeight + 10, propImg[0]);

	int dockPropX = riverEndWorldX - 100;   // change -100 to shift left/right from dock point
	int dockProp2Y = 300;   // prop2.png position (topmost)
	int dockProp6Y = 180;   // prop6.png position (middle)
	int dockProp4Y = 60;    // prop4.png position (bottommost)

	int dockScreenX = dockPropX - jungleWorldX;

	// Only draw when this position is actually visible on screen — so it never shows in jungle1/2/3
	if (dockScreenX > -propWidth && dockScreenX < 1000)
	{
		if ((dockProp2Y <= boatY) == frontPass)
			iShowImage(dockScreenX - 130, dockProp2Y - 160, propWidth, propHeight, propImg[1]);

		if ((dockProp6Y <= boatY) == frontPass)
			iShowImage(dockScreenX - 110, dockProp6Y - 2100, propWidth, propHeight, propImg[5]);

		if ((dockProp4Y <= boatY) == frontPass)
			iShowImage(dockScreenX - 110, dockProp4Y - 80, propWidth, propHeight, propImg[3]);
	}
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

	if (inBoat)   // fully seated — boat itself draws the rider now
		return;

	if (pickingUpPaper)
	{
		iShowImage(screenPlayerX, jungleScreenPlayerY, 120, 150, paperPickupImg[pickupPaperFrame]);
		return;
	}

	if (jumpingJungle)   // covers both rock-jumps AND boarding the boat
	{
		float jumpProgress = jungleJumpFrame / (float)(jumpFrameCount - 1);
		int jumpOffset = (int)(sinf(jumpProgress * 3.14159f) * jumpMaxHeight);
		iShowImage(screenPlayerX, jungleScreenPlayerY + jumpOffset, 120, 150, jumpImg[jungleJumpFrame]);
		return;
	}

	if (boardingBoat)   // fallback safety — shouldn't normally hit since boardingBoat only true while jumpingJungle
		return;

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
		if (paperCollected[i])
			continue;

		// While this paper is being picked up, keep showing the ground copy
		// for the first 2 pickup frames, then hide it.
		bool isBeingPicked = (i == activePaperIndex) && pickingUpPaper;
		if (isBeingPicked && pickupPaperFrame >= 2)
			continue;

		int screenX = paperWorldX[i] - jungleWorldX;
		if (screenX > -50 && screenX < 1050)
			iShowImage(screenX, paperY, 80, 80, paperImg);
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

	iShowImage(340, 200, 420, 330, msg4Img);   // TODO: position/size to match art
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

void updateBottles()
{
	if (!inBoat)
		return;

	for (int i = 0; i < bottleCount; i++)
	{
		if (bottleCollected[i])
			continue;

		int dx = abs((playerWorldX + 40) - bottleWorldX[i]);
		int dy = abs((boatY + 40) - bottleY[i]);

		if (dx < 60 && dy < 60)
		{
			bottleCollected[i] = true;
			hintsCollected++;
		}
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

	// Doll danger check 
	bool dollDangerMoving = isSpecialKeyPressed(GLUT_KEY_RIGHT) || isSpecialKeyPressed(GLUT_KEY_LEFT)
		|| pickingUpPaper || isSpecialKeyPressed(GLUT_KEY_DOWN);
	if (dollDangerMoving && !inBoat && !reachedRiverEnd && playerWorldX < jungleScreenWidth * 3)
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

	updateRiverWaves();
	updateBirds();
	updateBottles();
	updateAxe(playerWorldX);

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
				{
					paperCollected[activePaperIndex] = true;
					hintsCollected++;
				}
				activePaperIndex = -1;
			}
		}
		return;
	}

	if (isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		for (int i = 0; i < 3; i++)
		{
			if (paperCollected[i])
				continue;

			int diff = paperWorldX[i] - playerWorldX;   // positive = paper is to the right of player

			bool nearPaper;
			if (diff >= 0)
				nearPaper = diff < 150;   // player slightly LEFT of paper — wider tolerance
			else
				nearPaper = (-diff) < 60; // player passed the paper — normal tolerance

			if (nearPaper)
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
		// boarding check ekhon land korar somoy hobe (niche jumpingJungle block-e)
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

				// Jump land kore jekhane, shekhane check koro boat-er kache ache kina.
				// Ei number (boatBoardZone) barale/kamale "koto dure thakleo board hobe" seta control hobe.
				const int boatBoardZone = 150;
				if (!reachedRiverEnd && playerWorldX + 120 >= riverStartWorldX - boatBoardZone)
					boardingBoat = true;

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

		// Boat tree-ke cross korte parbe na, jodi na boat-er Y tree-r collision gap-er baire thake
		// (mane tree-r onek upor ba niche diye gele block hobe na).
		bool blockedByTree = false;
		if (inBoat)
		{
			for (int i = 0; i < treeCount; i++)
			{
				bool xOverlap = (playerWorldX + boatWidth > treeWorldX[i]) && (playerWorldX < treeWorldX[i] + treeWidth);
				bool yClose = abs(boatY - treeY[i]) < treeCollisionGapY;
				if (xOverlap && yClose)
					blockedByTree = true;
			}
		}

		int moveSpeed = jumpingJungle ? 10 : 5;

		if (playerWorldX < worldMax && !blockedByRock && !blockedByTree)
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
void drawHintCounter()
{
	char buffer[64];
	sprintf_s(buffer, sizeof(buffer), "Hint Collected: %d", hintsCollected);

	iSetColor(255, 255, 255);
	iText(10, 560, buffer, GLUT_BITMAP_HELVETICA_18);
}

#endif