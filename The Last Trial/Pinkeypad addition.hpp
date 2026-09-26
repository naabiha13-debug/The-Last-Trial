
#include <string.h>  

const char correctPin[4] = "731";   // the code the player must enter
char enteredPin[4] = "";            // digits typed so far, null-terminated
int enteredPinLen = 0;

bool pinWrong = false;      
bool pinUnlocked = false;  
bool pinUnlockedAcknowledged = false;

int pinUnlockedTimer = 0;
const int pinUnlockedDisplayFrames = 90;

struct PinButton { char digit; int cx, cy; };

// Centers of the 10 keypad buttons, in 1000x600 screen space (measured from pin.png).
PinButton pinButtons[10] = {
	{ '1', 399, 314 }, { '2', 500, 314 }, { '3', 600, 314 },
	{ '4', 399, 254 }, { '5', 500, 254 }, { '6', 600, 254 },
	{ '7', 399, 192 }, { '8', 500, 192 }, { '9', 600, 192 },
	{ '0', 500, 130 }
};

// Half-width / half-height of each button's clickable box (screen pixels).

const int pinBtnHalfW = 40;
const int pinBtnHalfH = 25;

void resetPinEntry()
{
	enteredPinLen = 0;
	enteredPin[0] = '\0';
	pinWrong = false;
	pinUnlocked = false;
	pinUnlockedAcknowledged = false;
	pinUnlockedTimer = 0;
}

void drawPinDisplay()
{
	if (!showPinFinal)
		return;

	iSetColor(120, 255, 160);

	if (pinWrong)
	{
		iText(430, 410, "WRONG", GLUT_BITMAP_TIMES_ROMAN_24); 
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

	iText(460, 410, spaced, GLUT_BITMAP_TIMES_ROMAN_24);   
}
void handlePinKeypadClick(int mx, int my)
{
	if (!showPinFinal)
		return;

	if (pinWrong)
	{
		resetPinEntry();
		return;
	}

	if (pinUnlocked)
	{
		showPinFinal = false;   // dismiss pin screen on next click
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
			return;
		}
	}
}   
void updatePinKeypad()
{
	if (!showPinFinal || !pinUnlocked)
		return;

	pinUnlockedTimer++;
	if (pinUnlockedTimer >= pinUnlockedDisplayFrames)
	{
		showPinFinal = false;
		pinUnlockedTimer = 0;
	}
}