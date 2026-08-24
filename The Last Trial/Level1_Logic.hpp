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
				bots[i].destChosen = false;
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

		int playerScore = player.score;
		int bot1Score = bots[0].score;
		int bot2Score = bots[1].score;

		// Find the lowest score
		int lowestScore = playerScore;

		if (bot1Score < lowestScore)
		{
			lowestScore = bot1Score;
		}

		if (bot2Score < lowestScore)
		{
			lowestScore = bot2Score;
		}

		// If YOU are the lowest, you lose.
		if (playerScore == lowestScore)
		{
			level1Result = RESULT_LOSE_TIME_UP;
		}
		else
		{
			// You are either 1st or 2nd.
			// Therefore YOU WIN.
			level1Result = RESULT_WIN_TIME_UP;
		}
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
		}

		return;
	}

	if (tilesVisible)
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
}

#endif
