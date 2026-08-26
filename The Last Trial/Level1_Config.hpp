#ifndef LEVEL1_CONFIG_HPP
#define LEVEL1_CONFIG_HPP

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
#define GRID_WIDTH 300
#define GRID_HEIGHT 300
#define GRID_X ((LEVEL1_WIDTH - GRID_WIDTH) / 2)
#define GRID_Y ((LEVEL1_HEIGHT - GRID_HEIGHT) / 2)
#define BOARD_X GRID_X
#define BOARD_Y GRID_Y
#define BOARD_WIDTH GRID_WIDTH
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

#define RESULT_NONE 0
#define RESULT_LOSE_RED_TILE 1
#define RESULT_LOSE_TIME_UP 2
#define RESULT_WIN_BOT_DIED 3
#define RESULT_WIN_TIME_UP 4
#define RESULT_DRAW_TIME_UP 5

// Forward declarations
struct Player;

void startLevel1();
void initLevel1();
void initializeCharacter(Player &p, int row, int col, bool bot);
void initializePlayers();

void updatePlayerAnimation();
void clearRedUnderCharacter(Player &p);
void claimTileIfGreen(Player &p);
void generateTiles();

bool isTileOccupied(int row, int col, Player *ignorePlayer);
bool isValidTile(int row, int col, Player *p);
bool isAdjacent(int fromRow, int fromCol, int toRow, int toCol);
bool isPathTileOpen(int row, int col, Player *ignorePlayer);
bool findPath(int startRow, int startCol, int destRow, int destCol,
	int outRow[ROWS * COLS], int outCol[ROWS * COLS], int &outLen, Player *p);

bool moveCharacterToTile(Player &p, int row, int col);
void checkCharacterTile(Player &p);
void updateCharacter(Player &p);
void updateTileVisibility();
void updateLevel1Timer();

int textPixelWidth(char *str, void *font);
void iTextCentered(int centerX, int y, char *str, void *font);
void drawTileGlow(int x, int y, int w, int h,
	int baseR, int baseG, int baseB,
	int liteR, int liteG, int liteB,
	int glowR, int glowG, int glowB);
void drawTiles();
void drawCharacter(Player &p, int botIndex);
void drawHUD();
void drawPhasePrompt();
void drawEndScreen();
void drawLevel1();

void movePlayerToTile(int row, int col);
void updatePlayerPath();
void botChooseDestination(Player &bot);
void updateBotPath(Player &bot);
void updateBots();
void level1Mouse(int button, int state, int mx, int my);
void level1Update();

extern int currentScreen;

// Shared Level 1 state
int tiles[ROWS][COLS];
bool tileClaimed[ROWS][COLS];

int backgroundImage;
int playerImg[4][2];
int botImg[BOT_COUNT][4][2];
int gameOverImage;
int victory1Image;
int victory2Image;
int qualifiedBot = 0;

int level1GameOver = 0;
int level1Time = GAME_TIME_DEFAULT;
bool level1TimerStarted = false;

clock_t tileTimerStart;
bool tilesVisible = true;
clock_t playerAnimTimerStart;
int level1Result = RESULT_NONE;

clock_t roundStartClock;
double level1PlayElapsedSeconds = 0.0;
clock_t level1PlaySegmentStart;
bool level1PlayTimerRunning = false;

int backBtnX = 0;
int backBtnY = 0;
int backBtnW = 0;
int backBtnH = 0;

#endif
