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

// Board floor এর সাইজ (এখন এটাই কালো ফ্লোরের সাইজ, ছবি না)
#define GRID_WIDTH  300
#define GRID_HEIGHT 300

// Board floor স্ক্রিনে কোথায় বসবে (ঠিক মাঝখানে, automatic)
#define GRID_X ((LEVEL1_WIDTH - GRID_WIDTH) / 2)
#define GRID_Y ((LEVEL1_HEIGHT - GRID_HEIGHT) / 2)

// এখন কোনো image border নেই, তাই padding = 0
// Board area = পুরো ফ্লোর area

#define BOARD_X GRID_X
#define BOARD_Y GRID_Y

#define BOARD_WIDTH  GRID_WIDTH
#define BOARD_HEIGHT GRID_HEIGHT

// TILE এর গড় সাইজ (player movement আর mouse click hit-testing এ ব্যবহার হবে)

#define TILE_WIDTH  (BOARD_WIDTH  / COLS)
#define TILE_HEIGHT (BOARD_HEIGHT / ROWS)


// ==========================================
// PRECISE TILE BOUNDARIES (rounding gap ছাড়াই)
// ==========================================
// Integer division করলে (BOARD_WIDTH / COLS) ভাগশেষ হারিয়ে যায়,
// ফলে শেষ column/row এ ছোট্ট ফাঁকা জায়গা থেকে যায়।
// এই ফাংশনগুলো প্রতিটা tile এর exact boundary বের করে,
// যাতে পুরো board area নিখুঁতভাবে ভরাট হয়, কোনো gap না থাকে।

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

#define WHITE 0
#define RED 1
#define GREEN 2

// Board এ RED/GREEN tile কত % আসবে (বাকিটা WHITE হবে)
// যেমন RED=15, GREEN=15 মানে 15% লাল, 15% সবুজ, 70% সাদা

#define RED_TILE_CHANCE 15
#define GREEN_TILE_CHANCE 15


// ==========================================
// PLAYER
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

	bool alive;

	int score;
};

Player player;


// ==========================================
// TILE ARRAY
// ==========================================

int tiles[ROWS][COLS];


// ==========================================
// IMAGE VARIABLES
// ==========================================

int backgroundImage;
int gridImage;
int playerImage;


// ==========================================
// GAME VARIABLES
// ==========================================

int level1GameOver = 0;
int level1Time = 30;


// ==========================================
// TILE COLOR REVEAL TIMER
// ==========================================

// রং কতক্ষণ দেখানো হবে (সেকেন্ডে)
#define TILE_VISIBLE_DURATION 2.0

// রং কতক্ষণ লুকানো থাকবে (সেকেন্ডে)
#define TILE_HIDDEN_DURATION 3.0

clock_t tileTimerStart;
bool tilesVisible = true;


// ==========================================
// LOAD LEVEL 1 IMAGES
// ==========================================

void initLevel1()
{
	backgroundImage =
		iLoadImage("Image//background.png");

	// grid.png আর ব্যবহার হচ্ছে না, plain কালো floor দিয়ে বদলানো হয়েছে
	// gridImage = iLoadImage("Image//grid.png");

	playerImage =
		iLoadImage("Image//player.png");
}


// ==========================================
// INITIALIZE PLAYER
// ==========================================

void initializePlayer()
{
	player.row = 4;
	player.col = 0;

	player.targetRow = 4;
	player.targetCol = 0;

	player.x = getTileCenterX(player.col);
	player.y = getTileCenterY(player.row);

	player.speed = 4;

	player.alive = true;

	player.score = 0;
}


// ==========================================
// GENERATE RANDOM TILES
// ==========================================

void generateTiles()
{
	int r;
	int c;

	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLS; c++)
		{
			// 0-99 এর মধ্যে random সংখ্যা, percentage হিসেবে ভাগ হবে

			int randomValue = rand() % 100;

			if (randomValue < RED_TILE_CHANCE)
			{
				tiles[r][c] = RED;
			}

			else if (randomValue < RED_TILE_CHANCE + GREEN_TILE_CHANCE)
			{
				tiles[r][c] = GREEN;
			}

			else
			{
				tiles[r][c] = WHITE;
			}
		}
	}


	// Starting tile is safe

	tiles[4][0] = GREEN;
}


// ==========================================
// UPDATE TILE COLOR REVEAL TIMER
// ==========================================

void updateTileVisibility()
{
	double elapsed =
		(double)(clock() - tileTimerStart) / CLOCKS_PER_SEC;

	if (tilesVisible)
	{
		if (elapsed >= TILE_VISIBLE_DURATION)
		{
			tilesVisible = false;
			tileTimerStart = clock();
		}
	}

	else
	{
		if (elapsed >= TILE_HIDDEN_DURATION)
		{
			// প্রতি cycle এ নতুন random pattern তৈরি হচ্ছে

			generateTiles();

			tilesVisible = true;
			tileTimerStart = clock();
		}
	}
}


// ==========================================
// DRAW TILES
// ==========================================

void drawTiles()
{
	int r;
	int c;

	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLS; c++)
		{
			int tileX = getTileLeft(c);
			int tileY = getTileTop(r);

			int tileW = getTileRight(c) - tileX;
			int tileH = getTileBottom(r) - tileY;


			// Hidden phase এ কিছুই আঁকা হবে না, grid image পুরোটা দেখা যাবে

			if (tilesVisible == false)
			{
				continue;
			}


			// Visible phase এ শুধু RED/GREEN tile এর উপর রং বসবে
			// WHITE tile এ কিছুই আঁকা হবে না, নিচের grid image দেখা যাবে

			if (tiles[r][c] == RED)
			{
				iSetColor(150, 25, 25); // dark red
			}

			else if (tiles[r][c] == GREEN)
			{
				iSetColor(20, 110, 45); // dark green
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


			// Tile এর চারপাশে হালকা কালো বর্ডার

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
// DRAW PLAYER
// ==========================================

void drawPlayer()
{
	if (player.alive == false)
	{
		return;
	}

	iShowImage(
		player.x - 30,
		player.y - 30,
		60,
		60,
		playerImage
		);
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

	// Grid image এর বদলে plain কালো floor

	iSetColor(15, 15, 15);

	iFilledRectangle(
		GRID_X,
		GRID_Y,
		GRID_WIDTH,
		GRID_HEIGHT
		);


	// পুরো board এর চারপাশে গাঢ় বর্ডার (frame)

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
	drawPlayer();
}
// ==========================================
// MOVE PLAYER TO CLICKED TILE
// ==========================================

void movePlayerToTile(
	int row,
	int col
	)
{
	player.targetRow = row;
	player.targetCol = col;
}


// ==========================================
// CHECK PLAYER'S TILE
// ==========================================

void checkPlayerTile()
{
	if (tiles[player.row][player.col] == RED)
	{
		player.alive = false;

		level1GameOver = 1;
	}

	else if (tiles[player.row][player.col] == GREEN)
	{
		player.score++;
	}

	else if (tiles[player.row][player.col] == WHITE)
	{
		// Nothing happens
	}
}


// ==========================================
// SMOOTH PLAYER MOVEMENT
// ==========================================

void updatePlayer()
{
	if (player.alive == false)
	{
		return;
	}


	int targetX = getTileCenterX(player.targetCol);
	int targetY = getTileCenterY(player.targetRow);


	float dx =
		targetX - player.x;

	float dy =
		targetY - player.y;


	float distance =
		sqrt(dx * dx + dy * dy);


	if (distance > 1)
	{
		player.x +=
			(dx / distance) * player.speed;

		player.y +=
			(dy / distance) * player.speed;
	}

	else
	{
		player.x = targetX;
		player.y = targetY;

		player.row = player.targetRow;
		player.col = player.targetCol;

		checkPlayerTile();
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

	if (level1GameOver == 1)
	{
		return;
	}

	// রং দেখা যাওয়ার সময় (memorize phase) ক্লিক করা যাবে না
	// রং লুকানো থাকলে (challenge phase) তখনই শুধু move করা যাবে

	if (tilesVisible)
	{
		return;
	}


	// Check whether click is inside board

	if (
		mx >= BOARD_X &&
		mx < BOARD_X + BOARD_WIDTH &&
		my >= BOARD_Y &&
		my < BOARD_Y + BOARD_HEIGHT
		)
	{
		int clickedCol =
			(int)((double)(mx - BOARD_X) * COLS / BOARD_WIDTH);

		int clickedRow =
			(int)((double)(my - BOARD_Y) * ROWS / BOARD_HEIGHT);


		movePlayerToTile(
			clickedRow,
			clickedCol
			);
	}
}


// ==========================================
// UPDATE LEVEL 1
// ==========================================

void level1Update()
{
	updateTileVisibility();

	updatePlayer();
}


// ==========================================
// START LEVEL 1
// ==========================================

void startLevel1()
{
	// rand() কে seed করা হচ্ছে, নাহলে প্রতিবার একই pattern আসে

	srand((unsigned int)(time(0) + clock()));

	initLevel1();

	initializePlayer();

	generateTiles();

	level1GameOver = 0;

	level1Time = 30;

	// Tile color reveal timer শুরু করা হচ্ছে

	tileTimerStart = clock();

	tilesVisible = true;
}

#endif