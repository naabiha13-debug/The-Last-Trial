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
	clock_t lastTileTime;
	bool hasClaimedGreenThisRound;
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
	p.lastTileTime = 0;
	p.hasClaimedGreenThisRound = false;
}

void initializePlayers()
{
	initializeCharacter(player, 4, 0, false);
	initializeCharacter(bots[0], 4, 1, true);
	initializeCharacter(bots[1], 4, 2, true);
}

void clearRedUnderCharacter(Player &p)
{
	if (!p.alive) return;

	if (p.row >= 0 && p.row < ROWS && p.col >= 0 && p.col < COLS)
	{
		if (tiles[p.row][p.col] == RED)
		{
			tiles[p.row][p.col] = WHITE;
		}
	}

	if (p.targetRow >= 0 && p.targetRow < ROWS && p.targetCol >= 0 && p.targetCol < COLS)
	{
		if (tiles[p.targetRow][p.targetCol] == RED)
		{
			tiles[p.targetRow][p.targetCol] = WHITE;
		}
	}

	if (p.prevRow >= 0 && p.prevRow < ROWS && p.prevCol >= 0 && p.prevCol < COLS)
	{
		if (tiles[p.prevRow][p.prevCol] == RED)
		{
			tiles[p.prevRow][p.prevCol] = WHITE;
		}
	}
}

void claimStandingTile(Player &p)
{
	if (!p.alive) return;

	if (p.row == p.targetRow && p.col == p.targetCol)
	{
		if (tiles[p.row][p.col] == GREEN && !tileClaimed[p.row][p.col])
		{
			p.score++;
			p.lastTileTime = clock();
			tileClaimed[p.row][p.col] = true;
			p.hasClaimedGreenThisRound = true;
		}
	}
}

bool playerMovedThisRound = false;

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

	playerMovedThisRound = false;
	player.hasClaimedGreenThisRound = false;
	for (int i = 0; i < BOT_COUNT; i++)
	{
		bots[i].hasClaimedGreenThisRound = false;
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

bool isContestantBetter(const Player &a, const Player &b)
{
	// Alive contestant beats dead contestant
	if (a.alive != b.alive)
	{
		return a.alive;
	}

	// Higher score beats lower score
	if (a.score != b.score)
	{
		return a.score > b.score;
	}

	// If scores are equal, whoever stepped on the green tile first wins
	if (a.score > 0)
	{
		return a.lastTileTime < b.lastTileTime;
	}

	return false;
}

void decideLevel1Result(bool botDiedTriggered)
{
	if (botDiedTriggered)
	{
		// A bot died, so player qualifies as long as player is alive
		level1Result = player.alive ? RESULT_WIN_BOT_DIED : RESULT_LOSE_RED_TILE;
		// The bot that survived is the qualified bot
		qualifiedBot = bots[0].alive ? 0 : 1;
		return;
	}

	// ৩ জনের স্কোর সমান হলে (৩-ওয়ে টাই), সবাই ফেইল্ড (Failed to Survive)
	if (player.score == bots[0].score && player.score == bots[1].score)
	{
		level1Result = RESULT_LOSE_TIME_UP;
		return;
	}

	// Count how many bots performed strictly better than the player
	int betterCount = 0;
	for (int i = 0; i < BOT_COUNT; i++)
	{
		if (isContestantBetter(bots[i], player))
		{
			betterCount++;
		}
	}

	// Top 2 qualify: if fewer than 2 contestants are better than the player, player qualifies!
	if (betterCount < 2)
	{
		level1Result = RESULT_WIN_TIME_UP;

		// Between the two bots, the one who performed better qualifies alongside player
		if (isContestantBetter(bots[0], bots[1]))
		{
			qualifiedBot = 0; // Bot 047
		}
		else
		{
			qualifiedBot = 1; // Bot 392
		}
	}
	else
	{
		level1Result = RESULT_LOSE_TIME_UP;
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

	if (tiles[p.row][p.col] == GREEN && !tileClaimed[p.row][p.col])
	{
		if (p.row == p.pathDestRow && p.col == p.pathDestCol)
		{
			p.score++;
			p.lastTileTime = clock();
			tileClaimed[p.row][p.col] = true;
			p.hasClaimedGreenThisRound = true;
			p.pathLen = 0;
			p.pathIdx = 0;
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
	if (player.hasClaimedGreenThisRound) return; // Cannot move if green tile already claimed this round!

	if (row != player.row || col != player.col)
	{
		playerMovedThisRound = true;
	}

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
	if (player.hasClaimedGreenThisRound) return; // Lock in place!

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