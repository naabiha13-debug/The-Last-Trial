// ============================================================================
// PIN KEYPAD LOGIC — paste this block into Level3_Jungle.hpp, ABOVE the
// existing handleJungleClick() function (it needs these declared first).
//
// Coordinates below were measured directly from pin.png (1620x971) and
// converted into the same 1000x600 screen space that
// iShowImage(0, 0, 1000, 600, pinImg) draws into. They should already line
// up reasonably well, but nudge pinBtnHalfW / pinBtnHalfH / the cx,cy values
// or the drawPinDisplay() text position if a button feels off — same as the
// other TODOs already in this file (e.g. controlRoomBtnX/Y/W/H).
// ============================================================================

#include <string.h>   // strcmp

const char correctPin[4] = "731";   // the code the player must enter
char enteredPin[4] = "";            // digits typed so far, null-terminated
int enteredPinLen = 0;

bool pinWrong = false;      // true once 3 digits were entered and were wrong
bool pinUnlocked = false;   // true once 3 digits were entered and matched

struct PinButton { char digit; int cx, cy; };

// Centers of the 10 keypad buttons, in 1000x600 screen space (measured from pin.png).
PinButton pinButtons[10] = {
	{ '1', 399, 314 }, { '2', 500, 314 }, { '3', 600, 314 },
	{ '4', 399, 254 }, { '5', 500, 254 }, { '6', 600, 254 },
	{ '7', 399, 192 }, { '8', 500, 192 }, { '9', 600, 192 },
	{ '0', 500, 130 }
};

// Half-width / half-height of each button's clickable box (screen pixels).
// TODO: fine-tune by testing in-game.
const int pinBtnHalfW = 40;
const int pinBtnHalfH = 25;

void resetPinEntry()
{
	enteredPinLen = 0;
	enteredPin[0] = '\0';
	pinWrong = false;
	pinUnlocked = false;
}

// Draws the digits typed so far (or WRONG / UNLOCKED) inside the small black
// LCD screen near the top of pin.png. Call this right after drawPinFinal().
void drawPinDisplay()
{
	if (!showPinFinal)
		return;

	iSetColor(120, 255, 160);

	if (pinWrong)
	{
		iText(430, 410, "WRONG", GLUT_BITMAP_TIMES_ROMAN_24);   // TODO: reposition to match the LCD
		return;
	}

	if (pinUnlocked)
	{
		iText(400, 410, "UNLOCKED", GLUT_BITMAP_TIMES_ROMAN_24);
		return;
	}

	char spaced[16] = "";
	int p = 0;
	for (int i = 0; i < enteredPinLen; i++)
	{
		spaced[p++] = enteredPin[i];
		spaced[p++] = ' ';
	}
	spaced[p] = '\0';

	iText(460, 410, spaced, GLUT_BITMAP_TIMES_ROMAN_24);   // TODO: reposition to match the LCD
}

// Call this at the very top of handleJungleClick(), before its existing steps.
void handlePinKeypadClick(int mx, int my)
{
	if (!showPinFinal)
		return;

	// After a wrong (or correct) attempt, the next click clears it for a retry.
	if (pinWrong || pinUnlocked)
	{
		resetPinEntry();
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		int dx = mx - pinButtons[i].cx;
		int dy = my - pinButtons[i].cy;

		if (dx > -pinBtnHalfW && dx < pinBtnHalfW && dy > -pinBtnHalfH && dy < pinBtnHalfH)
		{
			if (enteredPinLen < 3)
			{
				enteredPin[enteredPinLen] = pinButtons[i].digit;
				enteredPinLen++;
				enteredPin[enteredPinLen] = '\0';
			}

			if (enteredPinLen == 3)
			{
				if (strcmp(enteredPin, correctPin) == 0)
					pinUnlocked = true;
				else
					pinWrong = true;
			}
			return;   // stop checking other buttons once one was hit
		}
	}
}