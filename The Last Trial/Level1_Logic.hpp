#ifndef LEVEL1_LOGIC_HPP
#define LEVEL1_LOGIC_HPP
#include "Level1_Render.hpp"
void updateTileVisibility()
{
	double elapsed =
		(double)(clock() - tileTimerStart)
		/ CLOCKS_PER_SEC;
	if (tilesVisible)
	{
		if (elapsed >= currentTileVisibleDuration)
		{
			tilesVisible = false;

			for (int i = 0; i < BOT_COUNT; i++)
			{
				if (tiles[bots[i].row][bots[i].col] == GREEN)
				{
					// Bot is already on green: claim point and LOCK in place for this round!
					claimStandingTile(bots[i]);
					bots[i].pathLen = 0;
					bots[i].pathIdx = 0;
					bots[i].pathDestRow = bots[i].row;
					bots[i].pathDestCol = bots[i].col;
					bots[i].targetRow = bots[i].row;
					bots[i].targetCol = bots[i].col;
					bots[i].destChosen = true; // DO NOT move to other tiles!
				}
				else
				{
					// Bot is not on green: allow it to search and move to a tile
					bots[i].destChosen = false;
				}
			}

			tileTimerStart = clock();
			level1PlaySegmentStart = clock();
			level1PlayTimerRunning = true;
			level1TimerStarted = true;
		}
	}
	else
	{
		if (elapsed >= TILE_HIDDEN_DURATION)
		{
			if (level1PlayTimerRunning)
			{
				double segmentElapsed =
					(double)(clock() - level1PlaySegmentStart)
					/ CLOCKS_PER_SEC;
				level1PlayElapsedSeconds += segmentElapsed;
				level1PlayTimerRunning = false;
			}

			// If player stayed on starting green tile without moving, award point now!
			if (!playerMovedThisRound && !player.hasClaimedGreenThisRound)
			{
				claimStandingTile(player);
			}

			generateTiles();
			level1Round++;
			tilesVisible = true;
			tileTimerStart = clock();
		}
	}
}
void updateLevel1Timer()
{
	if (!level1TimerStarted)
	{
		return;
	}
	if (level1GameOver)
	{
		return;
	}
	double totalElapsed = level1PlayElapsedSeconds;
	if (level1PlayTimerRunning)
	{
		totalElapsed +=
			(double)(clock() - level1PlaySegmentStart)
			/ CLOCKS_PER_SEC;
	}
	int remaining =
		currentGameTime - (int)totalElapsed;
	if (remaining < 0)
	{
		remaining = 0;
	}
	level1Time = remaining;
	if (level1Time <= 0)
	{
		level1Time = 0;
		level1GameOver = 1;
		decideLevel1Result(false);
	}
}
void level1Mouse(
	int button,
	int state,
	int mx,
	int my)
{
	if (button != GLUT_LEFT_BUTTON)
	{
		return;
	}
	if (state != GLUT_DOWN)
	{
		return;
	}

	if (level1Phase == LEVEL1_PHASE_INTRO)
	{
		level1IntroMouse(button, state, mx, my);
		return;
	}

	if (level1Phase == LEVEL1_PHASE_COUNTDOWN)
	{
		return;
	}

	if (level1GameOver)
	{
		bool hitBack =
			mx >= backBtnX &&
			mx <= backBtnX + backBtnW &&
			my >= backBtnY &&
			my <= backBtnY + backBtnH;
		if (hitBack)
		{
			currentScreen = 2;
			playMenuMusic();
		}
		return;
	}
	if (tilesVisible || player.hasClaimedGreenThisRound)
	{
		return;
	}
	if (mx >= BOARD_X &&
		mx < BOARD_X + BOARD_WIDTH &&
		my >= BOARD_Y &&
		my < BOARD_Y + BOARD_HEIGHT)
	{
		int clickedCol =
			(int)(
			(double)(mx - BOARD_X)
			* COLS /
			BOARD_WIDTH);
		int clickedRow =
			(int)(
			(double)(my - BOARD_Y)
			* ROWS /
			BOARD_HEIGHT);
		movePlayerToTile(
			clickedRow,
			clickedCol);
	}
}
void level1Update()
{
	if (level1Phase == LEVEL1_PHASE_INTRO)
	{
		return;
	}

	if (level1Phase == LEVEL1_PHASE_COUNTDOWN)
	{
		updateLevel1Countdown();
		return;
	}

	if (level1GameOver)
	{
		return;
	}
	updateTileVisibility();
	updateLevel1Timer();
	if (level1GameOver)
	{
		return;
	}
	updateBots();
	updateCharacter(player);
	updatePlayerPath();
	for (int i = 0; i < BOT_COUNT; i++)
	{
		updateCharacter(bots[i]);
	}
	updatePlayerAnimation();
}
void startLevel1()
{
	srand(
		(unsigned int)(
		time(0) + clock()
		)
		);
	initLevel1();
	initializePlayers();
	generateTiles();
	level1GameOver = 0;
	level1Time = currentGameTime;
	level1Round = 1;
	level1Result = RESULT_NONE;
	level1TimerStarted = false;
	tileTimerStart = clock();
	tilesVisible = true;
	playerAnimTimerStart = clock();
	roundStartClock = clock();
	level1PlayElapsedSeconds = 0.0;
	level1PlayTimerRunning = false;
	playLevel1Music();
}
#endif