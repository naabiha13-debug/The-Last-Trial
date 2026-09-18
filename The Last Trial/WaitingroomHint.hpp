#ifndef WAITING_ROOM_HINT_HPP
#define WAITING_ROOM_HINT_HPP

#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>

bool wrHintActive = false;   // true while the player is choosing a vase
int  wrCorrectVase = 0;      // 0 = left vase, 1 = right vase (random)

// click boxes of the two vases (screen 1000 x 600)
int vaseX[2] = { 245, 271 };
const int vaseY = 281;
const int vaseW = 25;
const int vaseH = 46;

// call when "next" on warning2 is clicked (instead of starting the jungle)
void wrHintStart()
{
	srand((unsigned)time(NULL));
	wrCorrectVase = rand() % 2;
	wrHintActive = true;
}

// call on left mouse click. playerX = player's x in the waiting room.
// returns true only when the correct vase is clicked (the wrong vase does nothing)
bool wrHintMouse(int mx, int my, int playerX)
{
	if (!wrHintActive)
		return false;

	int i = wrCorrectVase;
	bool clicked = mx >= vaseX[i] && mx < vaseX[i] + vaseW && my >= vaseY && my < vaseY + vaseH;
	bool close = abs((playerX + 60) - (vaseX[i] + vaseW / 2)) < 90;   // player must stand near the vase

	if (clicked && close)
	{
		wrHintActive = false;
		return true;
	}
	return false;
}

#endif