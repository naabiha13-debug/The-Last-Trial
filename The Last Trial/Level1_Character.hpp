#ifndef LEVEL1_CHARACTER_HPP
#define LEVEL1_CHARACTER_HPP

#include "Level1_Config.hpp"

struct Player
{
	int x;
	int y;
	int row;
	int col;
	int targetRow;
	int targetCol;
	int speed;
	int facing;
	int animFrame;
	bool alive;
	int score;
	bool isBot;
	clock_t botTimer;
	int prevRow;
	int prevCol;
	int pathRow[ROWS * COLS];
	int pathCol[ROWS * COLS];
	int pathLen;
	int pathIdx;
	int pathDestRow, pathDestCol;
	clock_t pathStartClock;
	bool destChosen;
};

Player player;
Player bots[BOT_COUNT];

void updatePlayerAnimation()
{
	bool isMoving =
		(player.row != player.targetRow) ||
		(player.col != player.targetCol);

	if (isMoving == false)
	{
		player.animFrame = 0;
	}
	else
	{
		double elapsed =
			(double)(clock() - playerAnimTimerStart)
			/ CLOCKS_PER_SEC;

		if (elapsed >= ANIM_FRAME_DURATION)
		{
			player.animFrame = 1 - player.animFrame;
			playerAnimTimerStart = clock();
		}
	}

	for (int i = 0; i < BOT_COUNT; i++)
	{
		bool botMoving =
			(bots[i].row != bots[i].targetRow) ||
			(bots[i].col != bots[i].targetCol);

		if (botMoving == false)
		{
			bots[i].animFrame = 0;
		}
		else
		{
			double elapsed =
				(double)(clock() - bots[i].botTimer)
				/ CLOCKS_PER_SEC;

			if (elapsed >= ANIM_FRAME_DURATION)
			{
				bots[i].animFrame = 1 - bots[i].animFrame;
				bots[i].botTimer = clock();
			}
		}
	}
}

void initializeCharacter(Player &p, int row, int col, bool bot)
{
	p.row = row;
	p.col = col;
	p.targetRow = row;
	p.targetCol = col;
	p.x = getTileCenterX(col);
	p.y = getTileCenterY(row);
	p.speed = 4;
	p.alive = true;
	p.score = 0;
	p.facing = DIR_DOWN;
	p.animFrame = 0;
	p.isBot = bot;
	p.botTimer = clock();
	p.prevRow = row;
	p.prevCol = col;
	p.pathLen = 0;
	p.pathIdx = 0;
	p.pathDestRow = row;
	p.pathDestCol = col;
	p.pathStartClock = clock();
	p.destChosen = false;
}

void initializePlayers()
{
	initializeCharacter(player, 4, 0, false);
	initializeCharacter(bots[0], 4, 1, true);
	initializeCharacter(bots[1], 4, 2, true);
}

void clearRedUnderCharacter(Player &p)
{
	if (tiles[p.row][p.col] == RED)
	{
		tiles[p.row][p.col] = WHITE;
	}
}

void claimTileIfGreen(Player &p)
{
	if (tiles[p.row][p.col] == GREEN)
	{
		tileClaimed[p.row][p.col] = true;
	}
}

void generateTiles()
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			int randomValue = rand() % 100;

			if (randomValue < currentRedChance)
			{
				tiles[r][c] = RED;
			}
			else if (randomValue < currentRedChance + currentGreenChance)
			{
				tiles[r][c] = GREEN;
			}
			else
			{
				tiles[r][c] = WHITE;
			}

			tileClaimed[r][c] = false;
		}
	}

	clearRedUnderCharacter(player);

	for (int i = 0; i < BOT_COUNT; i++)
	{
		clearRedUnderCharacter(bots[i]);
	}

	claimTileIfGreen(player);

	for (int i = 0; i < BOT_COUNT; i++)
	{
		claimTileIfGreen(bots[i]);
	}

	player.pathLen = 0;
	player.pathIdx = 0;
}

bool moveCharacterToTile(Player &p, int row, int col)
{
	if (!isAdjacent(p.row, p.col, row, col))
	{
		return false;
	}

	if (!isValidTile(row, col, &p))
	{
		return false;
	}

	if (row > p.row)
	{
		p.facing = DIR_UP;
	}
	else if (row < p.row)
	{
		p.facing = DIR_DOWN;
	}
	else if (col > p.col)
	{
		p.facing = DIR_RIGHT;
	}
	else if (col < p.col)
	{
		p.facing = DIR_LEFT;
	}

	p.targetRow = row;
	p.targetCol = col;

	return true;
}

void decideLevel1Result(bool botDiedTriggered)
{
	int playerScore = player.score;
	int bot1Score = bots[0].score;
	int bot2Score = bots[1].score;

	int lowestScore = playerScore;
	if (bot1Score < lowestScore) lowestScore = bot1Score;
	if (bot2Score < lowestScore) lowestScore = bot2Score;

	if (playerScore == lowestScore)
	{
		level1Result = botDiedTriggered
			? RESULT_LOSE_RED_TILE
			: RESULT_LOSE_TIME_UP;
	}
	else
	{
		level1Result = botDiedTriggered
			? RESULT_WIN_BOT_DIED
			: RESULT_WIN_TIME_UP;
	}
}

void checkCharacterTile(Player &p)
{
	if (tiles[p.row][p.col] == RED)
	{
		if (p.isBot)
		{
			double sinceRoundStart =
				(double)(clock() - roundStartClock)
				/ CLOCKS_PER_SEC;

			if (sinceRoundStart < BOT_RED_IMMUNITY_SECONDS)
			{
				tiles[p.row][p.col] = WHITE;
				return;
			}
		}

		p.alive = false;
		level1GameOver = 1;

		if (p.isBot)
		{
			decideLevel1Result(true);
		}
		else
		{
			level1Result = RESULT_LOSE_RED_TILE;
		}

		return;
	}

	if (tiles[p.row][p.col] == GREEN)
	{
		if (p.row == p.pathDestRow && p.col == p.pathDestCol)
		{
			p.score++;
			tileClaimed[p.row][p.col] = true;
		}
	}
}

void updateCharacter(Player &p)
{
	if (!p.alive)
	{
		return;
	}

	bool needsToMove =
		(p.row != p.targetRow) ||
		(p.col != p.targetCol);

	if (!needsToMove)
	{
		return;
	}

	int targetX = getTileCenterX(p.targetCol);
	int targetY = getTileCenterY(p.targetRow);

	float dx = targetX - p.x;
	float dy = targetY - p.y;
	float distance = sqrt(dx * dx + dy * dy);

	if (distance > 1)
	{
		p.x += (dx / distance) * p.speed;
		p.y += (dy / distance) * p.speed;
	}
	else
	{
		p.x = targetX;
		p.y = targetY;

		p.prevRow = p.row;
		p.prevCol = p.col;

		p.row = p.targetRow;
		p.col = p.targetCol;

		checkCharacterTile(p);
	}
}

void movePlayerToTile(int row, int col)
{
	if (!player.alive) return;

	int fromRow = player.targetRow, fromCol = player.targetCol;
	int outRow[ROWS * COLS], outCol[ROWS * COLS], outLen;

	if (!findPath(fromRow, fromCol, row, col, outRow, outCol, outLen, &player)) return;

	for (int i = 0; i < outLen; i++)
	{
		player.pathRow[i] = outRow[i];
		player.pathCol[i] = outCol[i];
	}

	player.pathLen = outLen;
	player.pathIdx = 0;
	player.pathDestRow = row;
	player.pathDestCol = col;
}

void updatePlayerPath()
{
	if (!player.alive) return;

	bool arrived = (player.row == player.targetRow) &&
		(player.col == player.targetCol);

	if (!arrived) return;
	if (player.pathIdx >= player.pathLen) return;

	int nextRow = player.pathRow[player.pathIdx];
	int nextCol = player.pathCol[player.pathIdx];

	if (moveCharacterToTile(player, nextRow, nextCol))
	{
		player.pathIdx++;
		return;
	}

	int outRow[ROWS * COLS], outCol[ROWS * COLS], outLen;

	if (findPath(player.row, player.col, player.pathDestRow,
		player.pathDestCol, outRow, outCol, outLen, &player))
	{
		for (int i = 0; i < outLen; i++)
		{
			player.pathRow[i] = outRow[i];
			player.pathCol[i] = outCol[i];
		}

		player.pathLen = outLen;
		player.pathIdx = 0;
	}
	else
	{
		player.pathLen = 0;
		player.pathIdx = 0;
	}
}

#endif