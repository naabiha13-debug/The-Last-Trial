#include "iGraphics.h"
#include "menu.hpp"
#include "Level1.hpp"
#include <math.h>


int x = 0;
int y = 0;

int currentScreen = 0;

void iDraw()
{
	iClear();

	if (currentScreen == 0)
	{
		DrawMenu();
	}
	else if (currentScreen == 1)
	{
		drawLevel1();
	}
}



void iMouseMove(int mx, int my)
{
}


void iPassiveMouseMove(int mx, int my)
{
	if (currentScreen == 0)
	{
		if (mx >= 130 && mx <= 400 &&
			my >= 350 && my <= 430)
		{
			hoverButton = 1;
		}
		else if (mx >= 130 && mx <= 400 &&
			my >= 270 && my <= 350)
		{
			hoverButton = 2;
		}
		else if (mx >= 130 && mx <= 400 &&
			my >= 190 && my <= 270)
		{
			hoverButton = 3;
		}
		else if (mx >= 130 && mx <= 400 &&
			my >= 110 && my <= 190)
		{
			hoverButton = 4;
		}
		else
		{
			hoverButton = 0;
		}
	}
	else
	{
		hoverButton = 0;
	}
}


void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (currentScreen == 0)
		{
			// Play বাটনের কো-অর্ডিনেট (আপনার hoverButton==1 এর জায়গা অনুযায়ী)
			if (mx >= 130 && mx <= 400 &&
				my >= 350 && my <= 430)
			{
				currentScreen = 1;
				startLevel1();
			}
		}
		else if (currentScreen == 1)
		{
			level1Mouse(button, state, mx, my);
		}
	}
}
// Special Keys:
// GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, 
// GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT

void fixedUpdate()
{
	// ==========================================
	// LEVEL 1 UPDATE
	// ==========================================

	if (currentScreen == 1)
	{
		level1Update();
	}


	// ==========================================
	// EXISTING KEYBOARD CONTROLS
	// ==========================================

	if (isKeyPressed('w') || isSpecialKeyPressed(GLUT_KEY_UP))
	{
		y++;
	}

	if (isKeyPressed('a') || isSpecialKeyPressed(GLUT_KEY_LEFT))
	{
		x--;
	}

	if (isKeyPressed('s') || isSpecialKeyPressed(GLUT_KEY_DOWN))
	{
		y--;
	}

	if (isKeyPressed('d') || isSpecialKeyPressed(GLUT_KEY_RIGHT))
	{
		x++;
	}


	// ==========================================
	// ENTER
	// ==========================================

	if (isKeyPressed('\r'))
	{
		menuScreen = 1;
	}


	// ==========================================
	// SPACE - GAME OVER SOUND
	// ==========================================

	if (isKeyPressed(' '))
	{
		mciSendString(
			"play ggsong from 0",
			NULL,
			0,
			NULL
			);
	}
}

int main()
{
	// Opening/Loading the audio files
	mciSendString("open \"Audios//background.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//gameover.mp3\" alias ggsong", NULL, 0, NULL);

	// Playing the background audio on repeat
	mciSendString("play bgsong repeat", NULL, 0, NULL);

	// If the use of an audio is finished, close it to free memory
	// mciSendString("close bgsong", NULL, 0, NULL);
	// mciSendString("close ggsong", NULL, 0, NULL);
	

	iInitialize(1000, 600, "The Last Trial");
	initMenu();
	iStart();
	return 0;
}