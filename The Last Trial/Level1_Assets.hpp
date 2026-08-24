#ifndef LEVEL1_ASSETS_HPP
#define LEVEL1_ASSETS_HPP

#include "Level1_Bot.hpp"

void initLevel1()
{
	backgroundImage =
		iLoadImage("Image//background.png");

	playerImg[DIR_DOWN][0] =
		iLoadImage("Image//walk_down_1.png");
	playerImg[DIR_DOWN][1] =
		iLoadImage("Image//walk_down_2.png");

	playerImg[DIR_UP][0] =
		iLoadImage("Image//walk_up_1.png");
	playerImg[DIR_UP][1] =
		iLoadImage("Image//walk_up_2.png");

	playerImg[DIR_LEFT][0] =
		iLoadImage("Image//walk_left_1.png");
	playerImg[DIR_LEFT][1] =
		iLoadImage("Image//walk_left_2.png");

	playerImg[DIR_RIGHT][0] =
		iLoadImage("Image//walk_right_1.png");
	playerImg[DIR_RIGHT][1] =
		iLoadImage("Image//walk_right_2.png");

	botImg[0][DIR_DOWN][0] = iLoadImage("Image//bot1_walk_down_1.png");
	botImg[0][DIR_DOWN][1] = iLoadImage("Image//bot1_walk_down_2.png");
	botImg[0][DIR_UP][0] = iLoadImage("Image//bot1_walk_up_1.png");
	botImg[0][DIR_UP][1] = iLoadImage("Image//bot1_walk_up_2.png");
	botImg[0][DIR_LEFT][0] = iLoadImage("Image//bot1_walk_left_1.png");
	botImg[0][DIR_LEFT][1] = iLoadImage("Image//bot1_walk_left_2.png");
	botImg[0][DIR_RIGHT][0] = iLoadImage("Image//bot1_walk_right_1.png");
	botImg[0][DIR_RIGHT][1] = iLoadImage("Image//bot1_walk_right_2.png");

	botImg[1][DIR_DOWN][0] = iLoadImage("Image//bot2_walk_down_1.png");
	botImg[1][DIR_DOWN][1] = iLoadImage("Image//bot2_walk_down_2.png");
	botImg[1][DIR_UP][0] = iLoadImage("Image//bot2_walk_up_1.png");
	botImg[1][DIR_UP][1] = iLoadImage("Image//bot2_walk_up_2.png");
	botImg[1][DIR_LEFT][0] = iLoadImage("Image//bot2_walk_left_1.png");
	botImg[1][DIR_LEFT][1] = iLoadImage("Image//bot2_walk_left_2.png");
	botImg[1][DIR_RIGHT][0] = iLoadImage("Image//bot2_walk_right_1.png");
	botImg[1][DIR_RIGHT][1] = iLoadImage("Image//bot2_walk_right_2.png");

	gameOverImage = iLoadImage("Image//Game Over.png");
	victoryImage = iLoadImage("Image//Win.png");
}

#endif
