#include "iGraphics.h"
int storyPage = 1;
#include "menu.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include <math.h>
#include "Audio.hpp"
#include <stdlib.h>

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
	else if (currentScreen == 2)
	{
		DrawLevelSelection();
	}
	else if (currentScreen == 3)
	{
		DrawCredits();
	}
	else if (currentScreen == 4)
	{
		DrawStory();
	}
	else if (currentScreen == 5)
	{
		DrawLevel2();
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
	else if (currentScreen == 2)
	{
		if (mx >= 365 && mx <= 635 &&
			my >= 360 && my <= 440)
		{
			hoverLevel = 1;
		}
		else if (mx >= 365 && mx <= 635 &&
			my >= 260 && my <= 340)
		{
			hoverLevel = 2;
		}
		else if (mx >= 365 && mx <= 635 &&
			my >= 160 && my <= 240)
		{
			hoverLevel = 3;
		}
		else
		{
			hoverLevel = 0;
		}

	
	}
	else
	{
		hoverButton = 0;
		hoverLevel = 0;
	}
	
}


void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (currentScreen == 0 && menuScreen == 1)
		{
			if (hoverButton == 1)   // Play
			{
				currentScreen = 2;
			}
			else if (hoverButton == 2)   // Story
			{
				currentScreen = 4;
				storyPage = 1;
			}
			else if (hoverButton == 3)   // Credits
			{
				currentScreen = 3;
			}
			else if (hoverButton == 4)   // Exit
			{
				exit(0);
			}
		}
		else if (currentScreen == 2)
		{
			if (mx >= 365 && mx <= 635 &&
				my >= 360 && my <= 440)
			{
				// Level 1
				currentScreen = 1;
				enterLevel1();
				stopMenuMusic();
			}

			else if (mx >= 365 && mx <= 635 &&
				my >= 260 && my <= 340)
			{
				// Level 2
				currentScreen = 5;
				loadLevel2Assets();
				stopMenuMusic();
			}

			else if (mx >= 60 && mx <= 200 &&
				my >= 500 && my <= 550)
			{
				// BACK
				currentScreen = 0;
			}
		
		}
		else if (currentScreen == 3)
		{
			// BACK button
			if (mx >= 60 && mx <= 200 &&
				my >= 500 && my <= 550)
			{
				currentScreen = 0;
			}
		}
		else if (currentScreen == 4)
		{
			if (storyPage == 1)
			{
				// NEXT button
				if (mx >= 800 && mx <= 940 &&
					my >= 500 && my <= 550)
				{
					storyPage = 2;
				}
			}
			else if (storyPage == 2)
			{
				// NEXT button (was BACK)
				if (mx >= 800 && mx <= 940 &&
					my >= 500 && my <= 550)
				{
					storyPage = 3;
				}
			}
			else if (storyPage == 3)   // ADD THIS BLOCK
			{
				// BACK button
				if (mx >= 60 && mx <= 200 &&
					my >= 500 && my <= 550)
				{
					currentScreen = 0;
					storyPage = 1;
				}
			}
		}
		else if (currentScreen == 1)
		{
			// Level 1 mouse controls
			level1Mouse(button, state, mx, my);
		}
	}
}


// Special Keys:
// GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6, GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12, 
// GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP, GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT

void fixedUpdate()
{
	// Runs the level 1 game loop while that screen is active
	if (currentScreen == 1)
	{
		level1Update();
	}
	if (currentScreen == 5)
	{
		level2Update();
	}
	// Free-roam WASD/arrow controls (used outside level 1)
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

	// Enter advances from the title screen to the loading screen
	if (isKeyPressed('\r') && menuScreen == 0)
	{
		menuScreen = 3;
		loadingProgress = 0;
		loadingTimer = 0;
	}
	if (menuScreen == 3)
	{
		loadingTimer++;

		if (loadingTimer >= 1)
		{
			loadingTimer = 0;
			loadingProgress++;

			if (loadingProgress >= 100)
			{
				menuScreen = 1;
			}
		}
	}


	// Space is just a quick way to preview the game-over sound
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
	loadAudio();
	

	iInitialize(1000, 600, "The Last Trial");
	initMenu();
	iSetTimer(15, updateCharacterAnimation);
	iStart();
	return 0;
}