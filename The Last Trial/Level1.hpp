#ifndef LEVEL1_HPP
#define LEVEL1_HPP

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// Level 1 screen size

#define LEVEL1_WIDTH 1000
#define LEVEL1_HEIGHT 600

// Board / grid setup

#define ROWS 5
#define COLS 5

#define GRID_WIDTH  300
#define GRID_HEIGHT 300

#define GRID_X ((LEVEL1_WIDTH - GRID_WIDTH) / 2)
#define GRID_Y ((LEVEL1_HEIGHT - GRID_HEIGHT) / 2)

#define BOARD_X GRID_X
#define BOARD_Y GRID_Y

#define BOARD_WIDTH  GRID_WIDTH
#define BOARD_HEIGHT GRID_HEIGHT


int getTileLeft(int col)
{
	return BOARD_X + (int)((double)col * BOARD_WIDTH / COLS);
}

int getTileRight(int col)
{
	return BOARD_X + (int)((double)(col + 1) * BOARD_WIDTH / COLS);
}

int getTileTop(int row)
{
	return BOARD_Y + (int)((double)row * BOARD_HEIGHT / ROWS);
}

int getTileBottom(int row)
{
	return BOARD_Y + (int)((double)(row + 1) * BOARD_HEIGHT / ROWS);
}

int getTileCenterX(int col)
{
	return (getTileLeft(col) + getTileRight(col)) / 2;
}

int getTileCenterY(int row)
{
	return (getTileTop(row) + getTileBottom(row)) / 2;
}

// Tile types and facing directions

#define DIR_DOWN 0
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define WHITE 0
#define RED 1
#define GREEN 2


#define RED_TILE_CHANCE_DEFAULT 15
#define GREEN_TILE_CHANCE_DEFAULT 15

// Core game settings

#define BOT_COUNT 2

#define GAME_TIME_DEFAULT 50
#define BOT_RED_IMMUNITY_SECONDS 35.0

#define TILE_VISIBLE_DURATION_DEFAULT 2.0

#define ANIM_FRAME_DURATION 0.15

// How often a bot re-thinks its next move
#define BOT_MOVE_INTERVAL_MIN 0.5
#define BOT_MOVE_INTERVAL_MAX 1.3

#define BOT_MAX_TARGET_DISTANCE 6
#define BOT_MAX_TRAVEL_SECONDS 4.0

#define BOT_SMART_CHANCE_DEFAULT 90

#define TILE_HIDDEN_DURATION 3.0

int currentRedChance = RED_TILE_CHANCE_DEFAULT;
int currentGreenChance = GREEN_TILE_CHANCE_DEFAULT;
double currentTileVisibleDuration = TILE_VISIBLE_DURATION_DEFAULT;
int currentGameTime = GAME_TIME_DEFAULT;
int currentBotSmartChance = BOT_SMART_CHANCE_DEFAULT;

int level1Round = 1;

void startLevel1();

// Why the round ended, used to pick the right overlay/image
#define RESULT_NONE 0
#define RESULT_LOSE_RED_TILE 1
#define RESULT_LOSE_TIME_UP 2
#define RESULT_WIN_BOT_DIED 3
#define RESULT_WIN_TIME_UP 4
#define RESULT_DRAW_TIME_UP 5

// One player or bot on the board
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

// The human player and the bots sharing the board with them

Player player;

Player bots[BOT_COUNT];

// Tile colors for the current round
int tiles[ROWS][COLS];
bool tileClaimed[ROWS][COLS];

// Loaded textures

int backgroundImage;

int playerImg[4][2];
int botImg[BOT_COUNT][4][2];

int gameOverImage;
int victoryImage;

// Round state

int level1GameOver = 0;

int level1Time = GAME_TIME_DEFAULT;

bool level1TimerStarted = false;


clock_t tileTimerStart;

bool tilesVisible = true;

clock_t playerAnimTimerStart;

int level1Result = RESULT_NONE;

// phase ends.
clock_t roundStartClock;

double level1PlayElapsedSeconds = 0.0;
clock_t level1PlaySegmentStart;
bool level1PlayTimerRunning = false;

int backBtnX = 0;
int backBtnY = 0;
int backBtnW = 0;
int backBtnH = 0;

extern int currentScreen;

// Advances the walking animation frame for the player and every bot

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

// Loads every texture level 1 needs (background, walk cycles, end screens)

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

// Resets one character (player or bot) to a starting tile

void initializeCharacter(
	Player &p,
	int row,
	int col,
	bool bot
	)
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

// Places the player and both bots on their starting tiles

void initializePlayers()
{
	// Human player
	initializeCharacter(
		player,
		4,
		0,
		false
		);

	// Bot 1
	initializeCharacter(
		bots[0],
		4,
		1,
		true
		);

	// Bot 2
	initializeCharacter(
		bots[1],
		4,
		2,
		true
		);
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
			else if (
				randomValue <
				currentRedChance + currentGreenChance
				)
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
	if (
		row < 0 ||
		row >= ROWS ||
		col < 0 ||
		col >= COLS
		)
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


bool isAdjacent(
	int fromRow,
	int fromCol,
	int toRow,
	int toCol
	)
{
	int rowDifference = abs(toRow - fromRow);
	int colDifference = abs(toCol - fromCol);

	// Only one tile at a time
	if (
		(rowDifference == 1 && colDifference == 0) ||
		(rowDifference == 0 && colDifference == 1)
		)
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
	if (startRow == destRow && startCol == destCol) { outLen = 0; return true; }
	bool destIsRed = (tiles[destRow][destCol] == RED);
	if (!destIsRed && !isPathTileOpen(destRow, destCol, p)) return false;

	bool visited[ROWS][COLS];
	int parentRow[ROWS][COLS];
	int parentCol[ROWS][COLS];
	for (int r = 0; r < ROWS; r++) for (int c = 0; c < COLS; c++) visited[r][c] = false;

	int queueRow[ROWS * COLS], queueCol[ROWS * COLS];
	int qHead = 0, qTail = 0;
	visited[startRow][startCol] = true;
	queueRow[qTail] = startRow; queueCol[qTail] = startCol; qTail++;

	int dRow[4] = { 1, -1, 0, 0 };
	int dCol[4] = { 0, 0, -1, 1 };
	bool found = false;

	while (qHead < qTail)
	{
		int r = queueRow[qHead], c = queueCol[qHead]; qHead++;
		if (r == destRow && c == destCol) { found = true; break; }

		for (int i = 0; i < 4; i++)
		{
			int nr = r + dRow[i], nc = c + dCol[i];
			if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) continue;
			if (visited[nr][nc]) continue;
			bool isRedDestination = (nr == destRow) && (nc == destCol) && (tiles[nr][nc] == RED);
			if (!isRedDestination && !isPathTileOpen(nr, nc, p)) continue;

			visited[nr][nc] = true;
			parentRow[nr][nc] = r; parentCol[nr][nc] = c;
			queueRow[qTail] = nr; queueCol[qTail] = nc; qTail++;
		}
	}

	if (!found) return false;

	int tmpRow[ROWS * COLS], tmpCol[ROWS * COLS];
	int len = 0;
	int r = destRow, c = destCol;
	while (!(r == startRow && c == startCol))
	{
		tmpRow[len] = r; tmpCol[len] = c; len++;
		int pr = parentRow[r][c], pc = parentCol[r][c];
		r = pr; c = pc;
	}
	for (int i = 0; i < len; i++) { outRow[i] = tmpRow[len - 1 - i]; outCol[i] = tmpCol[len - 1 - i]; }
	outLen = len;
	return true;
}

bool moveCharacterToTile(
	Player &p,
	int row,
	int col
	)
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
			level1Result = RESULT_WIN_BOT_DIED;
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

	int targetX =
		getTileCenterX(p.targetCol);

	int targetY =
		getTileCenterY(p.targetRow);

	float dx =
		targetX - p.x;

	float dy =
		targetY - p.y;

	float distance =
		sqrt(dx * dx + dy * dy);

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
		int bestBotScore = 0;

		for (int i = 0; i < BOT_COUNT; i++)
		{
			if (bots[i].score > bestBotScore)
			{
				bestBotScore = bots[i].score;
			}
		}

		if (player.score > bestBotScore)
		{
			level1Result = RESULT_WIN_TIME_UP;
		}
		else if (player.score == bestBotScore)
		{
			level1Result = RESULT_DRAW_TIME_UP;
		}
		else
		{
			level1Result = RESULT_LOSE_TIME_UP;
		}
	}
}


int textPixelWidth(char *str, void *font)
{
	int width = 0;

	for (char *ch = str; *ch != '\0'; ch++)
	{
		width += glutBitmapWidth(font, (int)(unsigned char)*ch);
	}

	return width;
}

void iTextCentered(int centerX, int y, char *str, void *font)
{
	int width = textPixelWidth(str, font);

	iText(
		centerX - width / 2,
		y,
		str,
		font
		);
}


void drawTileGlow(
	int x,
	int y,
	int w,
	int h,
	int baseR, int baseG, int baseB,
	int liteR, int liteG, int liteB,
	int glowR, int glowG, int glowB
	)
{
	iSetColor(baseR, baseG, baseB);

	iFilledRectangle(x, y, w, h);

	int bandH = (int)(h * 0.5);

	iSetColor(liteR, liteG, liteB);

	iFilledRectangle(x, y + (h - bandH), w, bandH);

	iSetColor(glowR, glowG, glowB);

	iRectangle(x + 2, y + 2, w - 4, h - 4);
	iRectangle(x + 3, y + 3, w - 6, h - 6);

	iSetColor(10, 10, 10);

	iRectangle(x, y, w, h);
}

// Draws the colored tiles, but only while they're supposed to be visible

void drawTiles()
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			int tileX = getTileLeft(c);
			int tileY = getTileTop(r);

			int tileW =
				getTileRight(c) - tileX;

			int tileH =
				getTileBottom(r) - tileY;

			// Hide colors after the memorize phase ends
			if (!tilesVisible)
			{
				continue;
			}

			if (tiles[r][c] == RED)
			{
				// Base/lite/glow derived from the requested #bf211f
				drawTileGlow(
					tileX, tileY, tileW, tileH,
					86, 15, 14,
					191, 33, 31,
					255, 93, 91
					);
			}
			else if (tiles[r][c] == GREEN)
			{
				// Base/lite/glow derived from the requested #0c8247
				drawTileGlow(
					tileX, tileY, tileW, tileH,
					5, 59, 32,
					12, 130, 71,
					72, 190, 131
					);
			}
		}
	}
}

// Draws a single player or bot at its current animated position

void drawCharacter(Player &p, int botIndex = -1)
{
	if (!p.alive)
	{
		return;
	}

	int img;

	if (p.isBot && botIndex != -1)
	{
		img = botImg[botIndex][p.facing][p.animFrame];
	}
	else
	{
		img = playerImg[p.facing][p.animFrame];
	}

	iShowImage(
		p.x - 18,
		p.y - 24,
		36,
		48,
		img
		);
}


void drawHUD()
{
	int leftW = 250;
	int leftH = 56;

	int rightW = 130;
	int rightH = 56;

	int gap = 20;

	int groupWidth = leftW + gap + rightW;
	int groupX = (LEVEL1_WIDTH - groupWidth) / 2;

	int topMargin = 14;
	int panelY = LEVEL1_HEIGHT - leftH - topMargin;

	// ---- Left panel: green tiles collected so far, by everyone ----
	int leftX = groupX;
	int leftY = panelY;

	iSetColor(12, 6, 6);
	iFilledRectangle(leftX, leftY, leftW, leftH);

	iSetColor(90, 15, 15);
	iRectangle(leftX - 2, leftY - 2, leftW + 4, leftH + 4);

	iSetColor(190, 35, 35);
	iRectangle(leftX - 1, leftY - 1, leftW + 2, leftH + 2);

	iSetColor(255, 80, 80);
	iRectangle(leftX, leftY, leftW, leftH);

	int leftCenterX = leftX + leftW / 2;

	iSetColor(220, 110, 110);

	iTextCentered(
		leftCenterX,
		leftY + leftH - 18,
		"GREEN TILES COLLECTED",
		GLUT_BITMAP_8_BY_13
		);

	char greenCountsText[64];

	sprintf_s(
		greenCountsText,
		sizeof(greenCountsText),
		"YOU %d    BOT1 %d    BOT2 %d",
		player.score,
		bots[0].score,
		bots[1].score
		);

	iSetColor(255, 255, 255);

	iTextCentered(
		leftCenterX,
		leftY + 12,
		greenCountsText,
		GLUT_BITMAP_8_BY_13
		);

	// ---- Right panel: countdown timer ----
	int rightX = groupX + leftW + gap;
	int rightY = panelY;

	iSetColor(12, 6, 6);
	iFilledRectangle(rightX, rightY, rightW, rightH);

	iSetColor(90, 15, 15);
	iRectangle(rightX - 2, rightY - 2, rightW + 4, rightH + 4);

	iSetColor(190, 35, 35);
	iRectangle(rightX - 1, rightY - 1, rightW + 2, rightH + 2);

	iSetColor(255, 80, 80);
	iRectangle(rightX, rightY, rightW, rightH);

	int rightCenterX = rightX + rightW / 2;

	iSetColor(220, 110, 110);

	iTextCentered(
		rightCenterX,
		rightY + rightH - 18,
		"TIME",
		GLUT_BITMAP_8_BY_13
		);

	char timerText[16];

	sprintf_s(
		timerText,
		sizeof(timerText),
		"%d",
		level1Time
		);

	iSetColor(255, 255, 255);

	iTextCentered(
		rightCenterX,
		rightY + 10,
		timerText,
		GLUT_BITMAP_TIMES_ROMAN_24
		);
}


void drawPhasePrompt()
{
	
	char *text;

	if (tilesVisible)
	{
		iSetColor(230, 200, 90);
		text = "MEMORIZE THE TILES";
	}
	else
	{
		iSetColor(150, 220, 160);
		text = "GO! AVOID RED, GRAB GREEN";
	}

	iTextCentered(
		LEVEL1_WIDTH / 2,
		GRID_Y + GRID_HEIGHT + 26,
		text,
		GLUT_BITMAP_9_BY_15
		);
}

void drawEndScreen()
{
	bool won =
		(level1Result == RESULT_WIN_BOT_DIED) ||
		(level1Result == RESULT_WIN_TIME_UP);

	int img = won ? victoryImage : gameOverImage;

	int dispW = won ? 620 : 560;
	int dispH = won ? (int)(dispW * 793.0 / 1983.0)
		: (int)(dispW * 941.0 / 1672.0);

	int x = (LEVEL1_WIDTH - dispW) / 2;
	int y = (LEVEL1_HEIGHT - dispH) / 2 + 20;

	iShowImage(x, y, dispW, dispH, img);

	int btnWidth = 220;
	int btnHeight = 48;
	int btnX = (LEVEL1_WIDTH - btnWidth) / 2;
	int btnY = y - btnHeight - 20;

	iSetColor(20, 20, 20);
	iFilledRectangle(btnX, btnY, btnWidth, btnHeight);

	iSetColor(255, 255, 255);
	iRectangle(btnX, btnY, btnWidth, btnHeight);

	iTextCentered(
		btnX + btnWidth / 2,
		btnY + btnHeight / 2 - 6,
		"< BACK TO LEVELS",
		GLUT_BITMAP_9_BY_15
		);

	backBtnX = btnX;
	backBtnY = btnY;
	backBtnW = btnWidth;
	backBtnH = btnHeight;
}
void drawLevel1()
{
	iShowImage(
		0,
		0,
		1000,
		600,
		backgroundImage
		);

	drawTileGlow(GRID_X, GRID_Y, GRID_WIDTH, GRID_HEIGHT, 15, 15, 15, 28, 28, 28, 45, 45, 45);

	drawTiles();

	drawCharacter(player);

	for (int i = 0; i < BOT_COUNT; i++)
	{
		drawCharacter(bots[i], i);
	}

	drawHUD();

	if (!level1GameOver)
	{
		drawPhasePrompt();
	}

	if (level1GameOver)
	{
		drawEndScreen();
	}
}

void movePlayerToTile(int row, int col)
{
	if (!player.alive) return;
	int fromRow = player.targetRow, fromCol = player.targetCol;
	int outRow[ROWS * COLS], outCol[ROWS * COLS], outLen;

	if (!findPath(fromRow, fromCol, row, col, outRow, outCol, outLen, &player)) return;

	for (int i = 0; i < outLen; i++) { player.pathRow[i] = outRow[i]; player.pathCol[i] = outCol[i]; }
	player.pathLen = outLen;
	player.pathIdx = 0;
	player.pathDestRow = row;
	player.pathDestCol = col;
}


	void updatePlayerPath()
	{
		if (!player.alive) return;
		bool arrived = (player.row == player.targetRow) && (player.col == player.targetCol);
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
		if (findPath(player.row, player.col, player.pathDestRow, player.pathDestCol, outRow, outCol, outLen, &player))
		{
			for (int i = 0; i < outLen; i++) { player.pathRow[i] = outRow[i]; player.pathCol[i] = outCol[i]; }
			player.pathLen = outLen;
			player.pathIdx = 0;
		}
		else
		{
			player.pathLen = 0;
			player.pathIdx = 0;
		}
	}

	void botChooseDestination(Player &bot)
	{
		if (!bot.alive) return;

		double sinceRoundStart =
			(double)(clock() - roundStartClock) / CLOCKS_PER_SEC;
		bool botMustAvoidRed = sinceRoundStart < BOT_RED_IMMUNITY_SECONDS;

		int destRow = -1, destCol = -1;
		int outRow[ROWS * COLS], outCol[ROWS * COLS], outLen = -1;

		if (rand() % 100 < currentBotSmartChance)
		{
			for (int r = 0; r < ROWS; r++)
			for (int c = 0; c < COLS; c++)
			{
				if (tiles[r][c] != GREEN || tileClaimed[r][c]) continue;
				int tRow[ROWS * COLS], tCol[ROWS * COLS], tLen;
				if (!findPath(bot.row, bot.col, r, c, tRow, tCol, tLen, &bot)) continue;
				if (tLen > BOT_MAX_TARGET_DISTANCE) continue;
				if (outLen == -1 || tLen < outLen)
				{
					outLen = tLen;
					for (int i = 0; i < tLen; i++) { outRow[i] = tRow[i]; outCol[i] = tCol[i]; }
					destRow = r; destCol = c;
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

				vr[vCount] = pr[i]; vc[vCount] = pc[i]; vCount++;
			}

			if (vCount == 0)
			for (int i = 0; i < 4; i++)
			{
				if (!isValidTile(pr[i], pc[i], &bot)) continue;
				if (botMustAvoidRed && tiles[pr[i]][pc[i]] == RED) continue;

				vr[vCount] = pr[i]; vc[vCount] = pc[i]; vCount++;
			}

			if (vCount == 0) return;

			int pick = rand() % vCount;
			destRow = vr[pick]; destCol = vc[pick];
			if (!findPath(bot.row, bot.col, destRow, destCol, outRow, outCol, outLen, &bot)) return;
		}

		for (int i = 0; i < outLen; i++) { bot.pathRow[i] = outRow[i]; bot.pathCol[i] = outCol[i]; }
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

	if (moveCharacterToTile(bot, nextRow, nextCol)) bot.pathIdx++;
	else { bot.pathLen = 0; bot.pathIdx = 0; }
}

void updateBots()
{
	if (level1GameOver || tilesVisible) return;

	for (int i = 0; i < BOT_COUNT; i++)
	{
		if (!bots[i].alive) continue;
		updateBotPath(bots[i]);

		if (!bots[i].destChosen)
		{
			botChooseDestination(bots[i]);
			bots[i].destChosen = true;
		}
	}
}


void level1Mouse(
	int button,
	int state,
	int mx,
	int my
	)
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

	if (
		mx >= BOARD_X &&
		mx < BOARD_X + BOARD_WIDTH &&
		my >= BOARD_Y &&
		my < BOARD_Y + BOARD_HEIGHT
		)
	{
		int clickedCol =
			(int)(
			(double)(mx - BOARD_X)
			* COLS /
			BOARD_WIDTH
			);

		int clickedRow =
			(int)(
			(double)(my - BOARD_Y)
			* ROWS /
			BOARD_HEIGHT
			);

		movePlayerToTile(
			clickedRow,
			clickedCol
			);
	}
}

// Runs every frame while level 1 is active

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
