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

#define TILE_WIDTH  (BOARD_WIDTH / COLS)
#define TILE_HEIGHT (BOARD_HEIGHT / ROWS)

// Helpers to convert a row/col into exact pixel boundaries,
// so tiles line up perfectly even if the grid size changes later.

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

// Base/default values - actual values used in a round come from the
// difficulty-scaled variables below (see setLevel1Difficulty), so the
// three levels genuinely play differently instead of being the same
// board with a different label.
#define RED_TILE_CHANCE_DEFAULT 15
#define GREEN_TILE_CHANCE_DEFAULT 15

// Core game settings

#define BOT_COUNT 2

#define GAME_TIME_DEFAULT 50

// For this many seconds after a round starts, a bot is guaranteed to
// never die on a red tile (it can still happen to land on one and
// get bounced back safely). After this window, red tiles are
// dangerous for bots again, exactly like before.
#define BOT_RED_IMMUNITY_SECONDS 35.0

#define TILE_VISIBLE_DURATION_DEFAULT 2.0

#define ANIM_FRAME_DURATION 0.15

// How often a bot re-thinks its next move
#define BOT_MOVE_INTERVAL_MIN 0.5
#define BOT_MOVE_INTERVAL_MAX 1.3

// Chance (out of 100) that a bot picks a good/green tile instead of
// a random valid one. Kept below 100 on purpose so the bot still
// feels beatable and the round stays competitive with the player.
#define BOT_SMART_CHANCE_DEFAULT 80

#define TILE_HIDDEN_DURATION 3.0

// Difficulty levels, picked from the level-select screen
#define DIFFICULTY_EASY 0
#define DIFFICULTY_MEDIUM 1
#define DIFFICULTY_HARD 2

// Values actually used by the running round. Set by setLevel1Difficulty().
int level1Difficulty = DIFFICULTY_EASY;
int currentRedChance = RED_TILE_CHANCE_DEFAULT;
int currentGreenChance = GREEN_TILE_CHANCE_DEFAULT;
double currentTileVisibleDuration = TILE_VISIBLE_DURATION_DEFAULT;
int currentGameTime = GAME_TIME_DEFAULT;
int currentBotSmartChance = BOT_SMART_CHANCE_DEFAULT;

// How many memorize/hide cycles have happened so far this round -
// shown on the HUD so the player can feel the round actually
// progressing instead of it looking like the same static screen.
int level1Round = 1;

// Picks the tile chances / timings / bot skill for the chosen
// difficulty. Easy is forgiving (slower, more green tiles, dumber
// bots); Hard is tight and punishing (fast reveal, fewer green
// tiles, sharp bots) - so Level 1/2/3 are meaningfully different
// rounds, not the same level with a different button.
void setLevel1Difficulty(int difficulty)
{
	level1Difficulty = difficulty;

	if (difficulty == DIFFICULTY_EASY)
	{
		currentRedChance = 10;
		currentGreenChance = 20;
		currentTileVisibleDuration = 2.4;
		currentGameTime = 35;
		currentBotSmartChance = 60;
	}
	else if (difficulty == DIFFICULTY_HARD)
	{
		currentRedChance = 22;
		currentGreenChance = 12;
		currentTileVisibleDuration = 1.1;
		currentGameTime = 24;
		currentBotSmartChance = 92;
	}
	else // DIFFICULTY_MEDIUM
	{
		currentRedChance = 15;
		currentGreenChance = 16;
		currentTileVisibleDuration = 1.7;
		currentGameTime = 28;
		currentBotSmartChance = 80;
	}

	// The round clock is fixed at 50 seconds for every difficulty -
	// only the tile timing / bot skill above change with difficulty.
	currentGameTime = 50;
}

const char *level1DifficultyName()
{
	if (level1Difficulty == DIFFICULTY_EASY) return "EASY";
	if (level1Difficulty == DIFFICULTY_HARD) return "HARD";
	return "MEDIUM";
}

// Forward declaration - startLevel1() is defined at the bottom of this
// file, but level1Mouse() (defined earlier) needs it available.
void startLevel1(int difficulty = DIFFICULTY_EASY);

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

	// Timer used both for the bot's next-move delay and its walk animation
	clock_t botTimer;

	double nextDecisionDelay;

	int prevRow;
	int prevCol;

	// Queued path (used by the human player for click-to-move).
	// When the player clicks a tile that isn't a direct neighbour,
	// we compute a full route and walk it one tile at a time so the
	// movement always feels smooth instead of getting "stuck".
	int pathRow[ROWS * COLS];
	int pathCol[ROWS * COLS];
	int pathLen;
	int pathIdx;
};

// The human player and the bots sharing the board with them

Player player;

Player bots[BOT_COUNT];

// Tile colors for the current round
int tiles[ROWS][COLS];

// Once a GREEN (safe) tile has been stepped on, it's locked for the
// rest of the round - nobody else can move onto it, so player and
// bots are forced to spread out instead of stacking on one tile.
bool tileClaimed[ROWS][COLS];

// Loaded textures

int backgroundImage;

int playerImg[4][2];
int botImg[BOT_COUNT][4][2];

// Optional end-of-round artwork. These are only shown if the matching
// PNG actually exists in the Image folder - until then we fall back
// to a simple colored panel so the game never looks broken.
int gameOverImage;
bool gameOverImageAvailable = false;

int victoryImage;
bool victoryImageAvailable = false;

int drawImage;
bool drawImageAvailable = false;

// Round state

int level1GameOver = 0;

int level1Time = GAME_TIME_DEFAULT;

bool level1TimerStarted = false;

clock_t level1TimerStart;

// Tile visibility (memorize phase vs hidden phase)

clock_t tileTimerStart;

bool tilesVisible = true;

// Player walk-animation timer

clock_t playerAnimTimerStart;

// Result of the round, set the moment it's decided (see RESULT_* above)

int level1Result = RESULT_NONE;

// Counts every tile the human player has stepped onto this round -
// shown live on the HUD and again on the end-of-round summary.
int playerSteps = 0;

// Marks the instant the round actually began (used for the bot's
// red-tile immunity window and for the "time taken" stat), separate
// from level1TimerStart which only starts once the first memorize
// phase ends.
clock_t roundStartClock;

// How long the round lasted, frozen the moment it ends so the
// end-of-round box doesn't keep counting after the game is over.
int level1TimeTakenSeconds = 0;

// Bounds of the BACK TO LEVELS button drawn on the end-of-round box,
// refreshed every frame it's drawn so clicks can be tested against it.
int backBtnX = 0;
int backBtnY = 0;
int backBtnW = 0;
int backBtnH = 0;

// Defined in iMain.cpp - which top-level screen is showing (0 = main
// menu, 1 = this level, 2 = level select, ...). The end-of-round BACK
// button needs to set this back to the level-select screen.
extern int currentScreen;

// Returns true if the given file exists and can be opened for reading.
// Used to gracefully skip loading art assets that haven't been added yet.

bool fileExists(const char *path)
{
	FILE *file = NULL;

	fopen_s(&file, path, "rb");

	if (file != NULL)
	{
		fclose(file);
		return true;
	}

	return false;
}

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

	// End-of-round art is optional - drop these two files into the
	// Image folder whenever they're ready and they'll start showing
	// up automatically, no code changes needed.
	gameOverImageAvailable = fileExists("Image//gameover_screen.png");

	if (gameOverImageAvailable)
	{
		gameOverImage = iLoadImage("Image//gameover_screen.png");
	}

	victoryImageAvailable = fileExists("Image//victory_screen.png");

	if (victoryImageAvailable)
	{
		victoryImage = iLoadImage("Image//victory_screen.png");
	}

	drawImageAvailable = fileExists("Image//draw_screen.png");

	if (drawImageAvailable)
	{
		drawImage = iLoadImage("Image//draw_screen.png");
	}
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

	p.nextDecisionDelay =
		BOT_MOVE_INTERVAL_MIN +
		((double)(rand() % 100) / 100.0) *
		(BOT_MOVE_INTERVAL_MAX - BOT_MOVE_INTERVAL_MIN);
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

// If a character is standing on a tile that just rolled RED, that
// would kill them the instant the board regenerates - which isn't a
// fair "gotcha", so we clear it to a neutral WHITE tile instead.
// Unlike before, we do NOT force it to GREEN: doing that used to eat
// up most of the round's real green tiles (player + 2 bots = up to 3
// tiles auto-converted every round, out of only ~3-4 green tiles
// total), which is why bots kept ending up wandering onto plain
// tiles - there was rarely a real green tile left to walk to.

void clearRedUnderCharacter(Player &p)
{
	if (tiles[p.row][p.col] == RED)
	{
		tiles[p.row][p.col] = WHITE;
	}
}

// If a character happens to already be standing on a genuinely
// rolled GREEN tile, lock it in as claimed for them right away (it
// was never going to score for them, since they didn't walk onto it
// this round, but it should still count as theirs so nobody else can
// swoop in and take it out from under them).

void claimTileIfGreen(Player &p)
{
	if (tiles[p.row][p.col] == GREEN)
	{
		tileClaimed[p.row][p.col] = true;
	}
}

// Rolls a fresh tile layout for the round and clears every tile's
// "claimed" flag so green tiles become available again

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

	// The layout just changed completely (new memorize phase), so any
	// route the player had queued up no longer makes sense - clear it.
	player.pathLen = 0;
	player.pathIdx = 0;
}

// True if some other living player/bot is standing on this tile right
// now. Previously this also blocked a tile the moment someone else
// merely started heading toward it (their targetRow/targetCol), which
// meant the player could get blocked from a tile a bot was still a
// couple of steps away from - including tiles along a route the bot
// itself was walking. Now it only blocks on the tile someone is
// actually standing on.

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

// True if (row, col) is a legal destination for this character right now:
// on the board, not a green tile someone already claimed, and not
// currently occupied by someone else. Red tiles ARE a legal destination
// on purpose - stepping onto one is what actually triggers the
// red-tile death in checkCharacterTile(); blocking it here meant red
// tiles could never be reached at all.

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

	// A green tile that's already been claimed this round is off-limits
	// to everyone else, even once its original owner has moved away
	if (tiles[row][col] == GREEN && tileClaimed[row][col])
	{
		return false;
	}

	// Someone else is already there, or already moving there
	if (isTileOccupied(row, col, p))
	{
		return false;
	}

	return true;
}

// True if the two tiles are directly next to each other (no diagonals,
// no skipping over a tile in between)

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

// True if (row, col) is a tile a path is allowed to pass THROUGH -
// same rules as isValidTile but ignoring live occupancy, since
// bots/player keep moving while a path is being planned/walked.
// Occupancy is re-checked for real at the moment each step is taken.
// Red tiles are excluded here on purpose: they should never be used as
// a shortcut to hop across to somewhere else. They're still allowed as
// the actual final destination - see findPath(), which lets a route
// end on a red tile without ever routing through one.

bool isPathTileOpen(int row, int col)
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

	return true;
}

// Finds the shortest route (through open tiles only) from
// (startRow, startCol) to (destRow, destCol) using a breadth-first
// search over the small board grid. Fills outRow/outCol with the
// step-by-step tiles to walk through (not including the start tile)
// and outLen with how many steps that is. Returns false if there's
// no valid route at all.

bool findPath(
	int startRow,
	int startCol,
	int destRow,
	int destCol,
	int outRow[ROWS * COLS],
	int outCol[ROWS * COLS],
	int &outLen
	)
{
	if (startRow == destRow && startCol == destCol)
	{
		outLen = 0;
		return true;
	}

	// The destination itself is allowed to be a red tile (stepping onto
	// it is a legal, if fatal, move) - it just can't be a tile the
	// route passes through on the way to somewhere else.
	bool destIsRed = (tiles[destRow][destCol] == RED);

	if (!destIsRed && !isPathTileOpen(destRow, destCol))
	{
		return false;
	}

	bool visited[ROWS][COLS];
	int parentRow[ROWS][COLS];
	int parentCol[ROWS][COLS];

	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			visited[r][c] = false;
		}
	}

	int queueRow[ROWS * COLS];
	int queueCol[ROWS * COLS];
	int qHead = 0;
	int qTail = 0;

	visited[startRow][startCol] = true;
	queueRow[qTail] = startRow;
	queueCol[qTail] = startCol;
	qTail++;

	int dRow[4] = { 1, -1, 0, 0 };
	int dCol[4] = { 0, 0, -1, 1 };

	bool found = false;

	while (qHead < qTail)
	{
		int r = queueRow[qHead];
		int c = queueCol[qHead];
		qHead++;

		if (r == destRow && c == destCol)
		{
			found = true;
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			int nr = r + dRow[i];
			int nc = c + dCol[i];

			if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS)
			{
				continue;
			}

			if (visited[nr][nc])
			{
				continue;
			}

			// Same exception as above: the destination tile is allowed
			// through even if it's red, since a route is allowed to end
			// on one. Any other red tile is still a hard wall.
			bool isRedDestination =
				(nr == destRow) &&
				(nc == destCol) &&
				(tiles[nr][nc] == RED);

			if (!isRedDestination && !isPathTileOpen(nr, nc))
			{
				continue;
			}

			visited[nr][nc] = true;
			parentRow[nr][nc] = r;
			parentCol[nr][nc] = c;

			queueRow[qTail] = nr;
			queueCol[qTail] = nc;
			qTail++;
		}
	}

	if (!found)
	{
		return false;
	}

	// Walk the parent chain backwards from the destination, then
	// reverse it so outRow/outCol reads start -> destination.
	int tmpRow[ROWS * COLS];
	int tmpCol[ROWS * COLS];
	int len = 0;

	int r = destRow;
	int c = destCol;

	while (!(r == startRow && c == startCol))
	{
		tmpRow[len] = r;
		tmpCol[len] = c;
		len++;

		int pr = parentRow[r][c];
		int pc = parentCol[r][c];

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

// Starts a character moving toward (row, col), one adjacent tile at a
// time. Rejects the move if the tile isn't a direct neighbour, so
// nobody can hop across a red tile to reach a green one further away.

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

// Freezes level1TimeTakenSeconds the moment the round ends, measured
// from when the countdown actually started (not from the memorize
// phase before it), clamped to the round length.

void computeTimeTaken()
{
	if (level1TimerStarted)
	{
		double elapsed =
			(double)(clock() - level1TimerStart)
			/ CLOCKS_PER_SEC;

		int seconds = (int)elapsed;

		if (seconds < 0)
		{
			seconds = 0;
		}

		if (seconds > currentGameTime)
		{
			seconds = currentGameTime;
		}

		level1TimeTakenSeconds = seconds;
	}
	else
	{
		level1TimeTakenSeconds = 0;
	}
}

// Called the instant a character finishes arriving on a tile - handles
// dying on red, scoring on green, and claiming that green tile for
// the rest of the round

void checkCharacterTile(Player &p)
{
	if (tiles[p.row][p.col] == RED)
	{
		// For the opening BOT_RED_IMMUNITY_SECONDS of the round, a bot
		// is guaranteed to survive landing on a red tile - it's just
		// bounced back to a safe tile instead of dying. After that
		// window, red tiles are dangerous for bots again like normal.
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

		computeTimeTaken();

		// A bot dying on a red tile is an instant win for the player.
		// The player dying on one is an instant loss.
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
		p.score++;

		tileClaimed[p.row][p.col] = true;
	}
}

// Smoothly slides a character from its current tile toward its target
// tile, then locks it onto the target once it arrives

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

		if (!p.isBot)
		{
			playerSteps++;
		}

		checkCharacterTile(p);
	}
}

// Flips the tiles between "visible" (memorize the layout) and
// "hidden" (play from memory), regenerating the board each time a
// new memorize phase begins

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

			tileTimerStart = clock();

			// The main countdown must only start ONCE, the very first
			// time tiles go from visible to hidden. Every later
			// reveal/hide cycle (each new memorize phase) used to
			// reset level1TimerStart here too, which snapped the
			// visible timer back up near GAME_TIME every few seconds
			// instead of counting down smoothly. Guard it so it's
			// only set the first time.
			if (!level1TimerStarted)
			{
				level1TimerStarted = true;
				level1TimerStart = clock();
			}
		}
	}
	else
	{
		if (elapsed >= TILE_HIDDEN_DURATION)
		{
			generateTiles();

			level1Round++;

			tilesVisible = true;
			tileTimerStart = clock();
		}
	}
}

// Counts the round timer down and decides the outcome once it hits zero:
// the player wins only if they've collected more green tiles than every
// bot, otherwise it's a loss.

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
		currentGameTime - (int)elapsed;

	if (remaining < 0)
	{
		remaining = 0;
	}

	level1Time = remaining;

	if (level1Time <= 0)
	{
		level1Time = 0;

		level1GameOver = 1;

		computeTimeTaken();

		int bestBotScore = 0;

		for (int i = 0; i < BOT_COUNT; i++)
		{
			if (bots[i].score > bestBotScore)
			{
				bestBotScore = bots[i].score;
			}
		}

		// A tie is neither a win nor a loss - the player collected
		// exactly as many green tiles as the best bot, so calling
		// that a loss (like the old code did) wasn't fair.
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

// ---- Text helpers -------------------------------------------------
// iText always draws from a left-aligned x, which is why every prompt
// used to sit jammed against the left edge of the board. These two
// helpers measure a string in pixels (using the real GLUT bitmap
// widths) so text can be centered properly instead of eyeballed.

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

// Fills one tile with a soft two-tone shade (dark base + a lighter
// band) plus a bright inner glow edge, instead of one flat color -
// gives the red/green tiles some depth instead of looking like flat
// paint swatches.

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

// Draws the timer + step-counter panel to the right of the grid, in a
// glowing red-shaded box to match the trial's theme (no round/
// difficulty text - just the two live stats that matter mid-run).

// Red-glow HUD, drawn only while the round is actually in progress:
// green-tile counts for the player and both bots, and the countdown -
// both panels sit together at top middle instead of the opposite
// corners they used to be in, hidden once the round ends since the
// end-of-round box already covers this info.

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

// Big banner above the board telling the player which phase of the
// round they're in - memorizing the layout, or moving blind. This
// was a common point of confusion before (the only signal was the
// tiles silently disappearing), so it's now spelled out clearly.

void drawPhasePrompt()
{
	// iText on this library takes a plain char*, not const char* -
	// keeping these non-const avoids a C2664 compile error.
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

// Draws a small panel under the win/lose message showing how many
// tiles the player and each bot actually collected this round.

void drawScoreboard()
{
	int boxX = 250;
	int boxY = 420;
	int boxWidth = 500;
	int boxHeight = 140;

	iSetColor(8, 8, 8);

	iFilledRectangle(
		boxX,
		boxY,
		boxWidth,
		boxHeight
		);

	iSetColor(5, 5, 5);

	iRectangle(
		boxX,
		boxY,
		boxWidth,
		boxHeight
		);

	iRectangle(
		boxX - 1,
		boxY - 1,
		boxWidth + 2,
		boxHeight + 2
		);

	iSetColor(200, 170, 60);

	iText(
		boxX + 155,
		boxY + 115,
		"TILES COLLECTED",
		GLUT_BITMAP_9_BY_15
		);

	char line[64];

	iSetColor(255, 255, 255);

	sprintf_s(line, sizeof(line), "You: %d", player.score);

	iText(
		boxX + 45,
		boxY + 75,
		line,
		GLUT_BITMAP_9_BY_15
		);

	sprintf_s(line, sizeof(line), "Bot 1: %d", bots[0].score);

	iText(
		boxX + 205,
		boxY + 75,
		line,
		GLUT_BITMAP_9_BY_15
		);

	sprintf_s(line, sizeof(line), "Bot 2: %d", bots[1].score);

	iText(
		boxX + 365,
		boxY + 75,
		line,
		GLUT_BITMAP_9_BY_15
		);

	int bestBotScore = bots[0].score;

	if (bots[1].score > bestBotScore)
	{
		bestBotScore = bots[1].score;
	}

	char *standingText;

	if (player.score > bestBotScore)
	{
		standingText = "YOU COLLECTED THE MOST TILES";
	}
	else if (player.score == bestBotScore)
	{
		standingText = "IT'S A TIE ON TILES COLLECTED";
	}
	else
	{
		standingText = "A BOT COLLECTED THE MOST TILES";
	}

	iSetColor(180, 220, 255);

	iText(
		boxX + 100,
		boxY + 35,
		standingText,
		GLUT_BITMAP_8_BY_13
		);
}

// Draws a box with its four corners cut at 45 degrees (an octagon)
// instead of square corners, with a layered neon glow border and a
// short bright accent tick on each corner - the "sci-fi trial panel"
// look used for the end-of-round box.

void drawChamferedGlowBox(
	int x, int y, int w, int h, int chamfer,
	int bgR, int bgG, int bgB,
	int accentR, int accentG, int accentB
	)
{
	double px[8], py[8];

	px[0] = x + chamfer;	py[0] = y;
	px[1] = x + w - chamfer;	py[1] = y;
	px[2] = x + w;	py[2] = y + chamfer;
	px[3] = x + w;	py[3] = y + h - chamfer;
	px[4] = x + w - chamfer;	py[4] = y + h;
	px[5] = x + chamfer;	py[5] = y + h;
	px[6] = x;	py[6] = y + h - chamfer;
	px[7] = x;	py[7] = y + chamfer;

	iSetColor(bgR, bgG, bgB);
	iFilledPolygon(px, py, 8);

	// Faint scanlines across the interior for a digital-readout feel -
	// thin horizontal strips just a touch lighter than the background.
	iSetColor(bgR + 6, bgG + 6, bgB + 6);

	for (int scanY = y + 6; scanY < y + h - 6; scanY += 5)
	{
		iLine(x + chamfer / 2, scanY, x + w - chamfer / 2, scanY);
	}

	// Layered outline, dim to bright, each pass a little further out,
	// scaled from the accent color so a win and a loss can read as
	// different colors instead of everything always being red.
	for (int pass = 0; pass < 5; pass++)
	{
		int inset = 5 - pass;

		double scale[5] = { 0.25, 0.45, 0.70, 1.0, 1.0 };

		int lr = (int)(accentR * scale[pass]);
		int lg = (int)(accentG * scale[pass]);
		int lb = (int)(accentB * scale[pass]);

		double ox[8], oy[8];

		ox[0] = x + chamfer;	oy[0] = y - inset;
		ox[1] = x + w - chamfer;	oy[1] = y - inset;
		ox[2] = x + w + inset;	oy[2] = y + chamfer;
		ox[3] = x + w + inset;	oy[3] = y + h - chamfer;
		ox[4] = x + w - chamfer;	oy[4] = y + h + inset;
		ox[5] = x + chamfer;	oy[5] = y + h + inset;
		ox[6] = x - inset;	oy[6] = y + h - chamfer;
		ox[7] = x - inset;	oy[7] = y + chamfer;

		iSetColor(lr, lg, lb);
		iPolygon(ox, oy, 8);
	}

	// Bright accent ticks at each corner, sitting just outside the
	// chamfer cut - the small double-line marks from the reference art
	iSetColor(accentR, accentG, accentB);

	int tickLen = 26;
	int tickThick = 3;

	// Top-left
	iFilledRectangle(x - 2, y + h - 4, tickLen, tickThick);
	iFilledRectangle(x - 2, y + h - tickLen - 2, tickThick, tickLen);

	// Top-right
	iFilledRectangle(x + w + 2 - tickLen, y + h - 4, tickLen, tickThick);
	iFilledRectangle(x + w - 1, y + h - tickLen - 2, tickThick, tickLen);

	// Bottom-left
	iFilledRectangle(x - 2, y + 1, tickLen, tickThick);
	iFilledRectangle(x - 2, y + 2, tickThick, tickLen);

	// Bottom-right
	iFilledRectangle(x + w + 2 - tickLen, y + 1, tickLen, tickThick);
	iFilledRectangle(x + w - 1, y + 2, tickThick, tickLen);
}

// A row of short dashes instead of one solid line - reads a bit more
// like a digital terminal divider than a plain rule.

void drawDashedLine(
	int x1, int y, int x2,
	int accentR, int accentG, int accentB
	)
{
	iSetColor(accentR, accentG, accentB);

	int dashLen = 10;
	int gapLen = 7;

	for (int px = x1; px < x2; px += dashLen + gapLen)
	{
		int segEnd = px + dashLen;

		if (segEnd > x2)
		{
			segEnd = x2;
		}

		iLine(px, y, segEnd, y);
	}
}

// Draws the end-of-round overlay: one big glowing red box with the
// outcome, how long the round lasted, and how many tiles were
// stepped on - always custom-drawn (not the static end-screen PNGs)
// so the stats are real and there's never any icon/emoji baked in.

void drawGameOver()
{
	bool won =
		(level1Result == RESULT_WIN_BOT_DIED) ||
		(level1Result == RESULT_WIN_TIME_UP);

	// Win reads cyan-green, loss reads red - same panel design, but the
	// color itself now tells you the outcome at a glance instead of
	// everything always being red.
	int accentR, accentG, accentB;

	if (won)
	{
		accentR = 70;
		accentG = 235;
		accentB = 170;
	}
	else
	{
		accentR = 255;
		accentG = 70;
		accentB = 70;
	}

	int boxWidth = 580;
	int boxHeight = 460;

	int boxX = (LEVEL1_WIDTH - boxWidth) / 2;
	int boxY = (LEVEL1_HEIGHT - boxHeight) / 2;

	drawChamferedGlowBox(
		boxX, boxY, boxWidth, boxHeight, 34,
		8, 8, 8,
		accentR, accentG, accentB
		);

	int centerX = boxX + boxWidth / 2;

	// Small kicker line above the title, like a terminal log header
	iSetColor(
		(int)(accentR * 0.6),
		(int)(accentG * 0.6),
		(int)(accentB * 0.6)
		);

	iTextCentered(
		centerX,
		boxY + boxHeight - 48,
		won ? "// TRIAL STATUS: CLEARED" : "// TRIAL STATUS: TERMINATED",
		GLUT_BITMAP_8_BY_13
		);

	// Status indicator - a glowing dot in the top-left of the panel,
	// same idea as a status LED on a control panel
	int dotX = boxX + 46;
	int dotY = boxY + boxHeight - 46;

	iSetColor((int)(accentR * 0.3), (int)(accentG * 0.3), (int)(accentB * 0.3));
	iCircle(dotX, dotY, 11);

	iSetColor((int)(accentR * 0.6), (int)(accentG * 0.6), (int)(accentB * 0.6));
	iCircle(dotX, dotY, 7);

	iSetColor(accentR, accentG, accentB);
	iFilledCircle(dotX, dotY, 4);

	char *bigTitle;
	char *subTitle;

	if (won)
	{
		bigTitle = "CONGRATS";
		subTitle = "YOU SURVIVED";
	}
	else
	{
		bigTitle = "FAILED";
		subTitle = "YOU HAVE BEEN FAILED TO SURVIVE";
	}

	iSetColor(accentR, accentG, accentB);

	iTextCentered(
		centerX,
		boxY + boxHeight - 90,
		bigTitle,
		GLUT_BITMAP_TIMES_ROMAN_24
		);

	iSetColor(225, 225, 225);

	iTextCentered(
		centerX,
		boxY + boxHeight - 123,
		subTitle,
		GLUT_BITMAP_9_BY_15
		);

	drawDashedLine(
		boxX + 50,
		boxY + boxHeight - 150,
		boxX + boxWidth - 50,
		(int)(accentR * 0.45), (int)(accentG * 0.45), (int)(accentB * 0.45)
		);

	// TIME TAKEN
	iSetColor(200, 190, 120);

	iTextCentered(
		centerX,
		boxY + boxHeight - 190,
		"TIME TAKEN",
		GLUT_BITMAP_8_BY_13
		);

	char timeTakenText[32];

	sprintf_s(
		timeTakenText,
		sizeof(timeTakenText),
		"%d SEC",
		level1TimeTakenSeconds
		);

	iSetColor(255, 255, 255);

	iTextCentered(
		centerX,
		boxY + boxHeight - 223,
		timeTakenText,
		GLUT_BITMAP_TIMES_ROMAN_24
		);

	drawDashedLine(
		boxX + 50,
		boxY + boxHeight - 250,
		boxX + boxWidth - 50,
		(int)(accentR * 0.45), (int)(accentG * 0.45), (int)(accentB * 0.45)
		);

	// GREEN TILES COLLECTED - broken down by who collected what,
	// exactly like the live corner HUD but bigger, plain text only
	// (no cross/trophy icon of any kind)
	iSetColor(200, 190, 120);

	iTextCentered(
		centerX,
		boxY + boxHeight - 290,
		"GREEN TILES COLLECTED",
		GLUT_BITMAP_8_BY_13
		);

	char greenBreakdownText[64];

	sprintf_s(
		greenBreakdownText,
		sizeof(greenBreakdownText),
		"YOU %d    BOT1 %d    BOT2 %d",
		player.score,
		bots[0].score,
		bots[1].score
		);

	iSetColor(255, 255, 255);

	iTextCentered(
		centerX,
		boxY + boxHeight - 320,
		greenBreakdownText,
		GLUT_BITMAP_9_BY_15
		);

	int bestBotScore = bots[0].score;

	if (bots[1].score > bestBotScore)
	{
		bestBotScore = bots[1].score;
	}

	char *standingText;

	if (player.score > bestBotScore)
	{
		standingText = "YOU COLLECTED THE MOST GREEN TILES";
	}
	else if (player.score == bestBotScore)
	{
		standingText = "TIED WITH A BOT ON GREEN TILES";
	}
	else
	{
		standingText = "A BOT COLLECTED THE MOST GREEN TILES";
	}

	iSetColor(190, 190, 190);

	iTextCentered(
		centerX,
		boxY + boxHeight - 347,
		standingText,
		GLUT_BITMAP_8_BY_13
		);

	// BACK button - takes the player straight to the level-select
	// screen instead of restarting the same round.
	int btnWidth = 220;
	int btnHeight = 48;

	int btnX = centerX - btnWidth / 2;
	int btnY = boxY + 34;

	iSetColor(
		(int)(accentR * 0.14),
		(int)(accentG * 0.14),
		(int)(accentB * 0.14)
		);

	iFilledRectangle(btnX, btnY, btnWidth, btnHeight);

	iSetColor(accentR, accentG, accentB);

	iRectangle(btnX, btnY, btnWidth, btnHeight);
	iRectangle(btnX - 1, btnY - 1, btnWidth + 2, btnHeight + 2);

	iSetColor(255, 255, 255);

	iTextCentered(
		centerX,
		btnY + btnHeight / 2 - 6,
		"< BACK TO LEVELS",
		GLUT_BITMAP_9_BY_15
		);

	// Keep these current every frame so level1Mouse can hit-test clicks
	// against exactly where the button is actually drawn.
	backBtnX = btnX;
	backBtnY = btnY;
	backBtnW = btnWidth;
	backBtnH = btnHeight;
}

// Draws everything that makes up the level 1 screen

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
		drawCharacter(bots[i], i);
	}

	if (!level1GameOver)
	{
		drawHUD();

		drawPhasePrompt();
	}

	if (level1GameOver)
	{
		drawGameOver();

		// The tiles-collected panel is meant to be shown together with
		// the win/lose box (it was getting built but never actually
		// drawn) - restoring the call so it appears again like before.
		drawScoreboard();
	}
}

// Moves the human player toward the clicked tile (one step, if it's
// a legal move)

void movePlayerToTile(int row, int col)
{
	if (!player.alive)
	{
		return;
	}

	// Plan from wherever the player will finish their current step,
	// so clicking mid-walk continues smoothly instead of snapping.
	int fromRow = player.targetRow;
	int fromCol = player.targetCol;

	int outRow[ROWS * COLS];
	int outCol[ROWS * COLS];
	int outLen;

	bool hasPath = findPath(
		fromRow,
		fromCol,
		row,
		col,
		outRow,
		outCol,
		outLen
		);

	if (!hasPath)
	{
		// No valid route to that tile right now (a red or claimed-green
		// tile blocking the only way through, or it's off the board) -
		// ignore the click. Clicking a red tile itself is fine and will
		// path there - it's just fatal once the player arrives.
		return;
	}

	for (int i = 0; i < outLen; i++)
	{
		player.pathRow[i] = outRow[i];
		player.pathCol[i] = outCol[i];
	}

	player.pathLen = outLen;
	player.pathIdx = 0;
}

// Advances the player one step along its queued path once it has
// arrived at the current step's tile. If a step becomes blocked
// (e.g. a bot got there first, or the board just regenerated), the
// remaining path is cancelled instead of leaving the player stuck.

void updatePlayerPath()
{
	if (!player.alive)
	{
		return;
	}

	bool arrived =
		(player.row == player.targetRow) &&
		(player.col == player.targetCol);

	if (!arrived)
	{
		return;
	}

	if (player.pathIdx >= player.pathLen)
	{
		return;
	}

	int nextRow = player.pathRow[player.pathIdx];
	int nextCol = player.pathCol[player.pathIdx];

	if (moveCharacterToTile(player, nextRow, nextCol))
	{
		player.pathIdx++;
	}
	else
	{
		player.pathLen = 0;
		player.pathIdx = 0;
	}
}

// Picks the bot's next move. Most of the time it heads for a nearby
// green tile like a player would (an efficient, deliberate "pattern"
// rather than pure randomness); the rest of the time it takes any
// other valid step instead, so it stays beatable and the round feels
// like a real contest rather than the bot always winning.

void botChooseMove(Player &bot)
{
	if (!bot.alive)
	{
		return;
	}

	if (
		bot.row != bot.targetRow ||
		bot.col != bot.targetCol
		)
	{
		return;
	}

	int possibleRows[4] = { bot.row + 1, bot.row - 1, bot.row, bot.row };
	int possibleCols[4] = { bot.col, bot.col, bot.col - 1, bot.col + 1 };

	int validRows[4];
	int validCols[4];
	int validCount = 0;

	int greenRows[4];
	int greenCols[4];
	int greenCount = 0;

	for (int i = 0; i < 4; i++)
	{
		int r = possibleRows[i];
		int c = possibleCols[i];

		if (isValidTile(r, c, &bot))
		{
			// Avoid immediately backtracking to the tile just left,
			// so the bot doesn't shuffle back and forth in place
			bool isPrevTile = (r == bot.prevRow && c == bot.prevCol);

			if (isPrevTile)
			{
				continue;
			}

			validRows[validCount] = r;
			validCols[validCount] = c;
			validCount++;

			if (tiles[r][c] == GREEN)
			{
				greenRows[greenCount] = r;
				greenCols[greenCount] = c;
				greenCount++;
			}
		}
	}

	// If backtracking is the only option (dead end), allow it after all
	if (validCount == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			int r = possibleRows[i];
			int c = possibleCols[i];

			if (isValidTile(r, c, &bot))
			{
				validRows[validCount] = r;
				validCols[validCount] = c;
				validCount++;

				if (tiles[r][c] == GREEN)
				{
					greenRows[greenCount] = r;
					greenCols[greenCount] = c;
					greenCount++;
				}
			}
		}
	}

	if (validCount == 0)
	{
		return;
	}

	int chosenRow;
	int chosenCol;

	int roll = rand() % 100;

	if (roll < currentBotSmartChance && greenCount > 0)
	{
		int pick = rand() % greenCount;
		chosenRow = greenRows[pick];
		chosenCol = greenCols[pick];
	}
	else
	{
		int pick = rand() % validCount;
		chosenRow = validRows[pick];
		chosenCol = validCols[pick];
	}

	moveCharacterToTile(bot, chosenRow, chosenCol);

	bot.nextDecisionDelay =
		BOT_MOVE_INTERVAL_MIN +
		((double)(rand() % 100) / 100.0) *
		(BOT_MOVE_INTERVAL_MAX - BOT_MOVE_INTERVAL_MIN);
}

// Gives every bot a chance to move once its decision delay has passed

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

		if (elapsed >= bots[i].nextDecisionDelay)
		{
			botChooseMove(bots[i]);

			bots[i].botTimer = clock();
		}
	}
}

// Handles clicks on the board during level 1

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

	// Can't move while still memorizing the tile layout
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

// Resets and starts a fresh level 1 round.
// difficulty selects EASY / MEDIUM / HARD tuning (see setLevel1Difficulty);
// defaults to EASY so any old call site without the argument still compiles
// (the default is declared on the forward declaration near the top of
// this file - a default argument can only be specified once).

void startLevel1(int difficulty)
{
	srand(
		(unsigned int)(
		time(0) + clock()
		)
		);

	setLevel1Difficulty(difficulty);

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

	playerSteps = 0;

	level1TimeTakenSeconds = 0;

	roundStartClock = clock();
}

#endif
