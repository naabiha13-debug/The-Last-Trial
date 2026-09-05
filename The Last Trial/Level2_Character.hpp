#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "iGraphics.h"
#include "Level2_Config.hpp"
#include "Level2_Assets.hpp"
#include "Level2_Config.hpp"

int playerX = 100;
const int PLAYER_RUN_SPEED = 7;      
const int PLAYER_MOVE_DELAY = 80;       
DWORD playerMoveTime = 0;

// Defined later in Level2_Logic.hpp — forward-declared here so
// drawPlayer() can use it to pick the falling animation.
extern bool playerFalling;

// Ground-level Y (where the player stands/runs). Jump moves playerY
// above this and then returns it back here.
const int PLAYER_GROUND_Y = 401;
int playerY = PLAYER_GROUND_Y;

// =====================================
// PLAYER ANIMATION STATE
// =====================================

enum PlayerAnimState { PLAYER_IDLE, PLAYER_RUNNING, PLAYER_JUMPING };
PlayerAnimState playerAnimState = PLAYER_IDLE;

int playerRunFrame = 0;
DWORD playerRunFrameTime = 0;
const int PLAYER_RUN_FRAME_DELAY = 80; // ms per run frame

bool playerJumping = false;
int playerJumpFrame = 0;

DWORD playerJumpStartTime = 0;
const int PLAYER_JUMP_FRAME_DELAY = 100; // ms per jump frame (5 frames total)
const int PLAYER_JUMP_DURATION = 5 * PLAYER_JUMP_FRAME_DELAY; // total jump time
const float PLAYER_JUMP_HEIGHT = 70.0f; // how high the player rises, in pixels

bool jumpKeyWasDown = false; // for edge detection, so holding Up doesn't spam jumps


// === BISCUIT CATCH ANIMATION STATE ===
bool playerCatching = false;
DWORD playerCatchStartTime = 0;
const int PLAYER_CATCH_DURATION = 350; // ms per catch animation

void triggerPlayerCatch()
{
	playerCatching = true;
	playerCatchStartTime = GetTickCount();
}


void startPlayerJump()
{
	if (playerJumping)
		return;

	playerJumping = true;
	playerJumpFrame = 0;
	playerJumpStartTime = GetTickCount();
}

float getPlayerSpeedFactor();
// Call this every frame from updateLevel2(), passing whether the player
// is currently holding the move-right key.
void updatePlayerAnimation(bool isMovingRight)
{
	// Jump key (edge-triggered, so it fires once per press)
	bool jumpKeyDown = (GetAsyncKeyState(VK_UP) & 0x8000) ||
		(GetAsyncKeyState('W') & 0x8000);

	if (jumpKeyDown && !jumpKeyWasDown && !playerJumping)
	{
		startPlayerJump();
	}
	jumpKeyWasDown = jumpKeyDown;

	// Jump animation + motion takes priority over run/idle
	if (playerJumping)
	{
		DWORD elapsed = GetTickCount() - playerJumpStartTime;

		if (elapsed >= PLAYER_JUMP_DURATION)
		{
			// Landed — back to ground level
			playerJumping = false;
			playerJumpFrame = 0;
			playerY = PLAYER_GROUND_Y;
		}
		else
		{
			playerAnimState = PLAYER_JUMPING;

			playerJumpFrame = elapsed / PLAYER_JUMP_FRAME_DELAY;
			if (playerJumpFrame > 4)
				playerJumpFrame = 4;

			// Arc motion: 0 at takeoff, peak at mid-air, 0 at landing
			float progress = (float)elapsed / (float)PLAYER_JUMP_DURATION;
			float heightOffset = PLAYER_JUMP_HEIGHT * sinf(3.14159f * progress);

			playerY = PLAYER_GROUND_Y + (int)heightOffset;

			return;
		}
	}

	if (isMovingRight)
	{
		playerAnimState = PLAYER_RUNNING;

		DWORD now = GetTickCount();
		
		int animDelay = (int)(PLAYER_RUN_FRAME_DELAY / getPlayerSpeedFactor());
		if (now - playerRunFrameTime >= (DWORD)animDelay)
		{
			playerRunFrame = (playerRunFrame + 1) % 8;
			playerRunFrameTime = now;
		}
	}
	else
	{
		playerAnimState = PLAYER_IDLE;
		playerRunFrame = 0;
	}
}


void drawPlayer()
{
	// Clip drawing to the player's own viewport (y: 300-600)
	// so the falling animation never bleeds into the bot's screen
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, L2_VIEWPORT_HEIGHT, L2_SCREEN_WIDTH, L2_VIEWPORT_HEIGHT);

	int img;

	if (playerFalling)
	{
		// Time-based cycling, no separate timer state needed
		int fallFrame = (GetTickCount() / 120) % 3;
		img = playerFallImg[fallFrame];
	}
	else if (playerCatching)
	{
		DWORD elapsed = GetTickCount() - playerCatchStartTime;
		if (elapsed >= PLAYER_CATCH_DURATION)
		{
			playerCatching = false;
			if (playerAnimState == PLAYER_JUMPING)
				img = playerJumpImg[playerJumpFrame];
			else if (playerAnimState == PLAYER_RUNNING)
				img = playerRunImg[playerRunFrame];
			else
				img = playeridle;
		}
		else
		{
			int catchFrame = elapsed / (PLAYER_CATCH_DURATION / 2);
			if (catchFrame > 1)
				catchFrame = 1;
			img = playerCatchImg[catchFrame];
		}
	}
	else if (playerAnimState == PLAYER_JUMPING)
		img = playerJumpImg[playerJumpFrame];
	else if (playerAnimState == PLAYER_RUNNING)
		img = playerRunImg[playerRunFrame];
	else
		img = playeridle;

	iShowImage(playerX, playerY, 50, 80, img);

	glDisable(GL_SCISSOR_TEST);
}

float playerHealth = L2_HEALTH_MAX;
DWORD playerHealthDecayTime = 0;

float getPlayerSpeedFactor()
{
	float factor = playerHealth / L2_HEALTH_MAX;
	if (factor < L2_MIN_SPEED_FACTOR)   
		factor = L2_MIN_SPEED_FACTOR;
	return factor;
}

#endif