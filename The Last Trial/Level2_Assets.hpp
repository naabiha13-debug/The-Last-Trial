#ifndef LEVEL2_ASSET_HPP
#define LEVEL2_ASSET_HPP
#include "iGraphics.h"

int playerBg[5];
int botBg[5];
int bridgeTile;
int playeridle;

int playerRunImg[8];
int playerJumpImg[5];
int playerFallImg[3];
int botJumpImg[2][5];
int botFallImg[2][3];

int biscuitImg;
int playerCatchImg[2];

// bot1 (qualifiedBot == 0) and bot2 (qualifiedBot == 1) run cycles
int botRunImg[2][8];

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
	bridgeTile = iLoadImage("Image//g.png");

	playeridle = iLoadImage("Image//player_idle.png");

	playerRunImg[0] = iLoadImage("Image//player_run_frame_1.png");
	playerRunImg[1] = iLoadImage("Image//player_run_frame_2.png");
	playerRunImg[2] = iLoadImage("Image//player_run_frame_3.png");
	playerRunImg[3] = iLoadImage("Image//player_run_frame_4.png");
	playerRunImg[4] = iLoadImage("Image//player_run_frame_5.png");
	playerRunImg[5] = iLoadImage("Image//player_run_frame_6.png");
	playerRunImg[6] = iLoadImage("Image//player_run_frame_7.png");
	playerRunImg[7] = iLoadImage("Image//player_run_frame_8.png");

	playerJumpImg[0] = iLoadImage("Image//player_frame_1_jump_start.png");
	playerJumpImg[1] = iLoadImage("Image//player_frame_2_rising.png");
	playerJumpImg[2] = iLoadImage("Image//player_frame_3_peak.png");
	playerJumpImg[3] = iLoadImage("Image//player_frame_4_falling.png");
	playerJumpImg[4] = iLoadImage("Image//player_frame_5_landing.png");

	playerFallImg[0] = iLoadImage("Image//player_fall_frame_1.png");
	playerFallImg[1] = iLoadImage("Image//player_fall_frame_2.png");
	playerFallImg[2] = iLoadImage("Image//player_fall_frame_3.png");

	botRunImg[0][0] = iLoadImage("Image//bot1_run_1.png");
	botRunImg[0][1] = iLoadImage("Image//bot1_run_2.png");
	botRunImg[0][2] = iLoadImage("Image//bot1_run_3.png");
	botRunImg[0][3] = iLoadImage("Image//bot1_run_4.png");
	botRunImg[0][4] = iLoadImage("Image//bot1_run_5.png");
	botRunImg[0][5] = iLoadImage("Image//bot1_run_6.png");
	botRunImg[0][6] = iLoadImage("Image//bot1_run_7.png");
	botRunImg[0][7] = iLoadImage("Image//bot1_run_8.png");

	botRunImg[1][0] = iLoadImage("Image//bot2_run_1.png");
	botRunImg[1][1] = iLoadImage("Image//bot2_run_2.png");
	botRunImg[1][2] = iLoadImage("Image//bot2_run_3.png");
	botRunImg[1][3] = iLoadImage("Image//bot2_run_4.png");
	botRunImg[1][4] = iLoadImage("Image//bot2_run_5.png");
	botRunImg[1][5] = iLoadImage("Image//bot2_run_6.png");
	botRunImg[1][6] = iLoadImage("Image//bot2_run_7.png");
	botRunImg[1][7] = iLoadImage("Image//bot2_run_8.png");

	biscuitImg = iLoadImage("Image//biscuit.png");

	playerCatchImg[0] = iLoadImage("Image//player_catch1.png");
	playerCatchImg[1] = iLoadImage("Image//player_catch2.png");



	botJumpImg[0][0] = iLoadImage("Image//bot1_jump_1.png");
	botJumpImg[0][1] = iLoadImage("Image//bot1_jump_2.png");
	botJumpImg[0][2] = iLoadImage("Image//bot1_jump_3.png");
	botJumpImg[0][3] = iLoadImage("Image//bot1_jump_4.png");
	botJumpImg[0][4] = iLoadImage("Image//bot1_jump_5.png");

	botFallImg[0][0] = iLoadImage("Image//bot1_fall_1.png");
	botFallImg[0][1] = iLoadImage("Image//bot1_fall_2.png");
	botFallImg[0][2] = iLoadImage("Image//bot1_fall_3.png");

	botJumpImg[1][0] = iLoadImage("Image//bot2_jump_1.png");
	botJumpImg[1][1] = iLoadImage("Image//bot2_jump_2.png");
	botJumpImg[1][2] = iLoadImage("Image//bot2_jump_3.png");
	botJumpImg[1][3] = iLoadImage("Image//bot2_jump_4.png");
	botJumpImg[1][4] = iLoadImage("Image//bot2_jump_5.png");

	botFallImg[1][0] = iLoadImage("Image//bot2_fall_1.png");
	botFallImg[1][1] = iLoadImage("Image//bot2_fall_2.png");
	botFallImg[1][2] = iLoadImage("Image//bot2_fall_3.png");

}
#endif