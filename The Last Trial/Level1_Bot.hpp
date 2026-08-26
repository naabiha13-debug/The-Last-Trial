#ifndef LEVEL1_BOT_HPP
#define LEVEL1_BOT_HPP

#include "Level1_Character.hpp"

bool isTileOccupied(int row, int col, Player *ignorePlayer)
{
	if (&player != ignorePlayer && player.alive &&
		((player.row == row && player.col == col) ||
		(player.targetRow == row && player.targetCol == col)))
	{
		return true;
	}

	for (int i = 0; i < BOT_COUNT; i++)
	{
		if (&bots[i] != ignorePlayer && bots[i].alive &&
			((bots[i].row == row && bots[i].col == col) ||
			(bots[i].targetRow == row && bots[i].targetCol == col)))
		{
			return true;
		}
	}

	return false;
}

bool isValidTile(int row, int col, Player *p)
{
	if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
	{
		return false;
	}

	if (tiles[row][col] == GREEN && tileClaimed[row][col])
	{
		return false;
	}

	if (isTileOccupied(row, col, p))
	{
		return false;
	}

	return true;
}

bool isAdjacent(int fromRow, int fromCol, int toRow, int toCol)
{
	int rowDifference = abs(toRow - fromRow);
	int colDifference = abs(toCol - fromCol);

	if ((rowDifference == 1 && colDifference == 0) ||
		(rowDifference == 0 && colDifference == 1))
	{
		return true;
	}

	return false;
}

bool isPathTileOpen(int row, int col, Player *ignorePlayer)
{
	if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
	{
		return false;
	}

	if (tiles[row][col] == RED)
	{
		return false;
	}

	if (tiles[row][col] == GREEN && tileClaimed[row][col])
	{
		return false;
	}

	if (isTileOccupied(row, col, ignorePlayer))
	{
		return false;
	}

	return true;
}

bool findPath(int startRow, int startCol, int destRow, int destCol,
	int outRow[ROWS * COLS], int outCol[ROWS * COLS], int &outLen, Player *p)
{
	if (startRow == destRow && startCol == destCol)
	{
		outLen = 0;
		return true;
	}

	bool destIsRed = (tiles[destRow][destCol] == RED);

	if (!destIsRed && !isPathTileOpen(destRow, destCol, p))
	{
		return false;
	}

	bool visited[ROWS][COLS];
	int parentRow[ROWS][COLS];
	int parentCol[ROWS][COLS];

	for (int r = 0; r < ROWS; r++)
	for (int c = 0; c < COLS; c++)
		visited[r][c] = false;

	int queueRow[ROWS * COLS], queueCol[ROWS * COLS];
	int qHead = 0, qTail = 0;

	visited[startRow][startCol] = true;
	queueRow[qTail] = startRow;
	queueCol[qTail] = startCol;
	qTail++;

	int dRow[4] = { 1, -1, 0, 0 };
	int dCol[4] = { 0, 0, -1, 1 };
	bool found = false;

	while (qHead < qTail)
	{
		int r = queueRow[qHead], c = queueCol[qHead];
		qHead++;

		if (r == destRow && c == destCol)
		{
			found = true;
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			int nr = r + dRow[i], nc = c + dCol[i];

			if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) continue;
			if (visited[nr][nc]) continue;

			bool isRedDestination =
				(nr == destRow) && (nc == destCol) &&
				(tiles[nr][nc] == RED);

			if (!isRedDestination && !isPathTileOpen(nr, nc, p))
				continue;

			visited[nr][nc] = true;
			parentRow[nr][nc] = r;
			parentCol[nr][nc] = c;

			queueRow[qTail] = nr;
			queueCol[qTail] = nc;
			qTail++;
		}
	}

	if (!found) return false;

	int tmpRow[ROWS * COLS], tmpCol[ROWS * COLS];
	int len = 0;
	int r = destRow, c = destCol;

	while (!(r == startRow && c == startCol))
	{
		tmpRow[len] = r;
		tmpCol[len] = c;
		len++;

		int pr = parentRow[r][c], pc = parentCol[r][c];
		r = pr;
		c = pc;
	}

	for (int i = 0; i < len; i++)
	{
		outRow[i] = tmpRow[len - 1 - i];
		outCol[i] = tmpCol[len - 1 - i];
	}

	outLen = len;
	return true;
}

void botChooseDestination(Player &bot)
{
	if (!bot.alive) return;

	// If bot is already standing on a green tile, stay strictly in place!
	if (tiles[bot.row][bot.col] == GREEN)
	{
		bot.pathLen = 0;
		bot.pathIdx = 0;
		bot.pathDestRow = bot.row;
		bot.pathDestCol = bot.col;
		bot.targetRow = bot.row;
		bot.targetCol = bot.col;
		bot.destChosen = true;
		return;
	}

	double sinceRoundStart =
		(double)(clock() - roundStartClock) / CLOCKS_PER_SEC;

	bool botMustAvoidRed =
		sinceRoundStart < BOT_RED_IMMUNITY_SECONDS;

	int destRow = -1, destCol = -1;
	int outRow[ROWS * COLS], outCol[ROWS * COLS], outLen = -1;

	if (rand() % 100 < currentBotSmartChance)
	{
		for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
		{
			if (tiles[r][c] != GREEN || tileClaimed[r][c]) continue;

			int tRow[ROWS * COLS], tCol[ROWS * COLS], tLen;

			if (!findPath(bot.row, bot.col, r, c,
				tRow, tCol, tLen, &bot))
				continue;

			if (tLen > BOT_MAX_TARGET_DISTANCE) continue;

			if (outLen == -1 || tLen < outLen)
			{
				outLen = tLen;

				for (int i = 0; i < tLen; i++)
				{
					outRow[i] = tRow[i];
					outCol[i] = tCol[i];
				}

				destRow = r;
				destCol = c;
			}
		}
	}

	if (destRow == -1)
	{
		int pr[4] = { bot.row + 1, bot.row - 1, bot.row, bot.row };
		int pc[4] = { bot.col, bot.col, bot.col - 1, bot.col + 1 };

		int vr[4], vc[4], vCount = 0;

		for (int i = 0; i < 4; i++)
		{
			if (!isValidTile(pr[i], pc[i], &bot)) continue;
			if (botMustAvoidRed && tiles[pr[i]][pc[i]] == RED) continue;
			if (pr[i] == bot.prevRow && pc[i] == bot.prevCol) continue;

			vr[vCount] = pr[i];
			vc[vCount] = pc[i];
			vCount++;
		}

		if (vCount == 0)
		for (int i = 0; i < 4; i++)
		{
			if (!isValidTile(pr[i], pc[i], &bot)) continue;
			if (botMustAvoidRed && tiles[pr[i]][pc[i]] == RED) continue;

			vr[vCount] = pr[i];
			vc[vCount] = pc[i];
			vCount++;
		}

		if (vCount == 0) return;

		int pick = rand() % vCount;
		destRow = vr[pick];
		destCol = vc[pick];

		if (!findPath(bot.row, bot.col, destRow, destCol,
			outRow, outCol, outLen, &bot))
			return;
	}

	for (int i = 0; i < outLen; i++)
	{
		bot.pathRow[i] = outRow[i];
		bot.pathCol[i] = outCol[i];
	}

	bot.pathLen = outLen;
	bot.pathIdx = 0;
	bot.pathDestRow = destRow;
	bot.pathDestCol = destCol;
	bot.pathStartClock = clock();
}

void updateBotPath(Player &bot)
{
	if (!bot.alive) return;

	if (bot.pathIdx < bot.pathLen &&
		(double)(clock() - bot.pathStartClock) / CLOCKS_PER_SEC >= BOT_MAX_TRAVEL_SECONDS)
	{
		bot.pathLen = 0;
		bot.pathIdx = 0;
		return;
	}

	if (bot.row != bot.targetRow || bot.col != bot.targetCol) return;
	if (bot.pathIdx >= bot.pathLen) return;

	int nextRow = bot.pathRow[bot.pathIdx];
	int nextCol = bot.pathCol[bot.pathIdx];

	if (moveCharacterToTile(bot, nextRow, nextCol))
		bot.pathIdx++;
	else
	{
		bot.pathLen = 0;
		bot.pathIdx = 0;
	}
}

void updateBots()
{
	if (level1GameOver || tilesVisible) return;

	for (int i = 0; i < BOT_COUNT; i++)
	{
		if (!bots[i].alive) continue;
		if (bots[i].hasClaimedGreenThisRound) continue; // Cannot move anymore if already stepped on green this round!

		updateBotPath(bots[i]);

		if (!bots[i].destChosen)
		{
			botChooseDestination(bots[i]);
			bots[i].destChosen = true;
		}
	}
}

#endif
