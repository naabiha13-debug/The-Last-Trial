#ifndef LEVEL1_HPP
#define LEVEL1_HPP

#include <stdlib.h>
#include <time.h>
#include <math.h>


// ==========================================
// LEVEL 1 SCREEN
// ==========================================

#define LEVEL1_WIDTH 1000
#define LEVEL1_HEIGHT 600


// ==========================================
// BOARD
// ==========================================

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

#define TILE_WIDTH  (BOARD_WIDTH / COLS)
#define TILE_HEIGHT (BOARD_HEIGHT / ROWS)


// ==========================================
// PRECISE TILE BOUNDARIES
// ==========================================

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


// ==========================================
// TILE TYPES
// ==========================================

#define DIR_DOWN 0
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define WHITE 0
#define RED 1
#define GREEN 2

#define RED_TILE_CHANCE 15
#define GREEN_TILE_CHANCE 15


// ==========================================
// GAME SETTINGS
// ==========================================

#define BOT_COUNT 2

#define GAME_TIME 30

#define TILE_VISIBLE_DURATION 2.0

#define ANIM_FRAME_DURATION 0.15

// Bot কতক্ষণ পর পর move করবে
#define BOT_MOVE_INTERVAL 0.75


// ==========================================
// PLAYER STRUCTURE
// ==========================================

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

	// Bot-এর movement timer
	clock_t botTimer;
};


// ==========================================
// PLAYERS
// ==========================================

Player player;

Player bots[BOT_COUNT];


// ==========================================
// TILE ARRAY
// ==========================================

int tiles[ROWS][COLS];


// ==========================================
// IMAGES
// ==========================================

int backgroundImage;

int playerImg[4][2];


// ==========================================
// GAME VARIABLES
// ==========================================

int level1GameOver = 0;

int level1Time = GAME_TIME;

bool level1TimerStarted = false;

clock_t level1TimerStart;


// ==========================================
// TILE VISIBILITY
// ==========================================

clock_t tileTimerStart;

bool tilesVisible = true;


// ==========================================
// PLAYER ANIMATION TIMER
// ==========================================

clock_t playerAnimTimerStart;


// ==========================================
// GAME OVER / WINNER
// ==========================================

int winnerType = 0;
// 0 = none
// 1 = player survived
// 2 = bot survived
// 3 = multiple survived


// ==========================================
// UPDATE PLAYER ANIMATION
// ==========================================

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


// ==========================================
// LOAD IMAGES
// ==========================================

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
}


// ==========================================
// INITIALIZE ONE CHARACTER
// ==========================================

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
}


// ==========================================
// INITIALIZE PLAYER + BOTS
// ==========================================

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


// ==========================================
// GENERATE RANDOM TILES
// ==========================================

void generateTiles()
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			int randomValue = rand() % 100;

			if (randomValue < RED_TILE_CHANCE)
			{
				tiles[r][c] = RED;
			}
			else if (
				randomValue <
				RED_TILE_CHANCE + GREEN_TILE_CHANCE
				)
			{
				tiles[r][c] = GREEN;
			}
			else
			{
				tiles[r][c] = WHITE;
			}
		}
	}

	// Starting positions must be safe
	tiles[4][0] = GREEN;
	tiles[4][1] = GREEN;
	tiles[4][2] = GREEN;
}


// ==========================================
// CHECK WHETHER TILE IS OCCUPIED
// ==========================================

bool isTileOccupied(int row, int col, Player *ignorePlayer)
{
	if (
		&player != ignorePlayer &&
		player.alive &&
		player.row == row &&
		player.col == col
		)
	{
		return true;
	}

	for (int i = 0; i < BOT_COUNT; i++)
	{
		if (
			&bots[i] != ignorePlayer &&
			bots[i].alive &&
			bots[i].row == row &&
			bots[i].col == col
			)
		{
			return true;
		}
	}

	return false;
}


// ==========================================
// CHECK VALID TILE
// ==========================================

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

	// Red tile cannot be entered
	if (tiles[row][col] == RED)
	{
		return false;
	}

	// White tile is not safe
	if (tiles[row][col] != GREEN)
	{
		return false;
	}

	// Another player/bot is already there
	if (isTileOccupied(row, col, p))
	{
		return false;
	}

	return true;
}


// ==========================================
// CHECK WHETHER MOVEMENT IS ADJACENT
// ==========================================

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


// ==========================================
// MOVE CHARACTER
// ==========================================

bool moveCharacterToTile(
	Player &p,
	int row,
	int col
	)
{
	// Must move only one tile at a time
	if (
		!isAdjacent(
		p.row,
		p.col,
		row,
		col
		)
		)
	{
		return false;
	}

	// Target must be green and unoccupied
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


// ==========================================
// CHECK CHARACTER TILE
// ==========================================

void checkCharacterTile(Player &p)
{
	if (tiles[p.row][p.col] == RED)
	{
		p.alive = false;

		level1GameOver = 1;

		return;
	}

	if (tiles[p.row][p.col] == GREEN)
	{
		p.score++;
	}
}


// ==========================================
// UPDATE CHARACTER MOVEMENT
// ==========================================

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

		p.row = p.targetRow;
		p.col = p.targetCol;

		checkCharacterTile(p);
	}
}


// ==========================================
// UPDATE TILE VISIBILITY
// ==========================================

void updateTileVisibility()
{
	double elapsed =
		(double)(clock() - tileTimerStart)
		/ CLOCKS_PER_SEC;

	if (tilesVisible)
	{
		if (elapsed >= TILE_VISIBLE_DURATION)
		{
			tilesVisible = false;

			tileTimerStart = clock();

			// Timer starts when challenge begins
			level1TimerStarted = true;

			level1TimerStart = clock();
		}
	}
	else
	{
		// IMPORTANT:
		// Do NOT generate new tiles every cycle.
		// The same safe pattern remains for the whole round.
	}
}


// ==========================================
// UPDATE GAME TIMER
// ==========================================

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

	double elapsed =
		(double)(clock() - level1TimerStart)
		/ CLOCKS_PER_SEC;

	int remaining =
		GAME_TIME - (int)elapsed;

	if (remaining < 0)
	{
		remaining = 0;
	}

	level1Time = remaining;

	if (level1Time <= 0)
	{
		level1Time = 0;

		level1GameOver = 1;

		// Determine survivors
		bool playerAlive = player.alive;

		bool botAlive = false;

		for (int i = 0; i < BOT_COUNT; i++)
		{
			if (bots[i].alive)
			{
				botAlive = true;
			}
		}

		if (playerAlive && botAlive)
		{
			winnerType = 3;
		}
		else if (playerAlive)
		{
			winnerType = 1;
		}
		else if (botAlive)
		{
			winnerType = 2;
		}
		else
		{
			winnerType = 0;
		}
	}
}


// ==========================================
// DRAW TILES
// ==========================================

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

			// Hide colors after memorization
			if (!tilesVisible)
			{
				continue;
			}

			if (tiles[r][c] == RED)
			{
				iSetColor(150, 25, 25);
			}
			else if (tiles[r][c] == GREEN)
			{
				iSetColor(20, 110, 45);
			}
			else
			{
				continue;
			}

			iFilledRectangle(
				tileX,
				tileY,
				tileW,
				tileH
				);

			iSetColor(30, 30, 30);

			iRectangle(
				tileX,
				tileY,
				tileW,
				tileH
				);
		}
	}
}


// ==========================================
// DRAW ONE CHARACTER
// ==========================================

void drawCharacter(Player &p)
{
	if (!p.alive)
	{
		return;
	}

	int img =
		playerImg[p.facing][p.animFrame];

	iShowImage(
		p.x - 18,
		p.y - 24,
		36,
		48,
		img
		);
}


// ==========================================
// DRAW TIMER
// ==========================================

void drawTimer()
{
	iSetColor(255, 255, 255);

	char timerText[50];

	sprintf(
		timerText,
		"TIME: %d",
		level1Time
		);

	iText(
		40,
		550,
		timerText,
		GLUT_BITMAP_HELVETICA_18
		);
}


// ==========================================
// DRAW GAME OVER
// ==========================================

void drawGameOver()
{
	iSetColor(180, 0, 0);

	iFilledRectangle(
		250,
		230,
		500,
		140
		);

	iSetColor(255, 255, 255);

	iText(
		410,
		310,
		"GAME OVER",
		GLUT_BITMAP_HELVETICA_18
		);

	if (level1Time <= 0)
	{
		iText(
			390,
			275,
			"TIME UP!",
			GLUT_BITMAP_HELVETICA_18
			);
	}
}


// ==========================================
// DRAW LEVEL 1
// ==========================================

void drawLevel1()
{
	iShowImage(
		0,
		0,
		1000,
		600,
		backgroundImage
		);

	iSetColor(15, 15, 15);

	iFilledRectangle(
		GRID_X,
		GRID_Y,
		GRID_WIDTH,
		GRID_HEIGHT
		);

	iSetColor(5, 5, 5);

	iRectangle(
		GRID_X,
		GRID_Y,
		GRID_WIDTH,
		GRID_HEIGHT
		);

	iRectangle(
		GRID_X - 1,
		GRID_Y - 1,
		GRID_WIDTH + 2,
		GRID_HEIGHT + 2
		);

	iRectangle(
		GRID_X - 2,
		GRID_Y - 2,
		GRID_WIDTH + 4,
		GRID_HEIGHT + 4
		);

	drawTiles();

	drawCharacter(player);

	for (int i = 0; i < BOT_COUNT; i++)
	{
		drawCharacter(bots[i]);
	}

	drawTimer();

	if (level1GameOver)
	{
		drawGameOver();
	}
}


// ==========================================
// HUMAN PLAYER MOVEMENT
// ==========================================

void movePlayerToTile(int row, int col)
{
	if (!player.alive)
	{
		return;
	}

	moveCharacterToTile(
		player,
		row,
		col
		);
}


// ==========================================
// BOT AI
// ==========================================

void botChooseMove(Player &bot)
{
	if (!bot.alive)
	{
		return;
	}

	// Already moving
	if (
		bot.row != bot.targetRow ||
		bot.col != bot.targetCol
		)
	{
		return;
	}

	int possibleRows[4] =
	{
		bot.row + 1,
		bot.row - 1,
		bot.row,
		bot.row
	};

	int possibleCols[4] =
	{
		bot.col,
		bot.col,
		bot.col - 1,
		bot.col + 1
	};

	int validRows[4];

	int validCols[4];

	int validCount = 0;

	for (int i = 0; i < 4; i++)
	{
		int r = possibleRows[i];
		int c = possibleCols[i];

		if (
			isValidTile(
			r,
			c,
			&bot
			)
			)
		{
			validRows[validCount] = r;
			validCols[validCount] = c;

			validCount++;
		}
	}

	if (validCount == 0)
	{
		return;
	}

	// 75% chance to make a normal safe decision
	// 25% chance to choose a random valid move
	int choice;

	int randomValue = rand() % 100;

	if (randomValue < 75)
	{
		choice = rand() % validCount;
	}
	else
	{
		choice = rand() % validCount;
	}

	moveCharacterToTile(
		bot,
		validRows[choice],
		validCols[choice]
		);
}


// ==========================================
// UPDATE BOTS
// ==========================================

void updateBots()
{
	if (level1GameOver)
	{
		return;
	}

	if (tilesVisible)
	{
		return;
	}

	for (int i = 0; i < BOT_COUNT; i++)
	{
		if (!bots[i].alive)
		{
			continue;
		}

		double elapsed =
			(double)(clock() - bots[i].botTimer)
			/ CLOCKS_PER_SEC;

		if (elapsed >= BOT_MOVE_INTERVAL)
		{
			botChooseMove(bots[i]);

			bots[i].botTimer = clock();
		}
	}
}


// ==========================================
// LEVEL 1 MOUSE
// ==========================================

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
		return;
	}

	// Cannot move during memorization phase
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


// ==========================================
// LEVEL 1 UPDATE
// ==========================================

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

	for (int i = 0; i < BOT_COUNT; i++)
	{
		updateCharacter(bots[i]);
	}

	updatePlayerAnimation();
}


// ==========================================
// START LEVEL 1
// ==========================================

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

	level1Time = GAME_TIME;

	winnerType = 0;

	level1TimerStarted = false;

	tileTimerStart = clock();

	tilesVisible = true;

	playerAnimTimerStart = clock();
}

#endif