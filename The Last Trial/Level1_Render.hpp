#ifndef LEVEL1_RENDER_HPP
#define LEVEL1_RENDER_HPP

#include "Level1_Assets.hpp"

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

	iText(centerX - width / 2, y, str, font);
}

#include "Level1_Intro.hpp"

void drawTileGlow(
	int x, int y, int w, int h,
	int baseR, int baseG, int baseB,
	int liteR, int liteG, int liteB,
	int glowR, int glowG, int glowB)
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

void drawTiles()
{
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			int tileX = getTileLeft(c);
			int tileY = getTileTop(r);

			int tileW = getTileRight(c) - tileX;
			int tileH = getTileBottom(r) - tileY;

			if (!tilesVisible)
			{
				continue;
			}

			if (tiles[r][c] == RED)
			{
				drawTileGlow(
					tileX, tileY, tileW, tileH,
					86, 15, 14,
					191, 33, 31,
					255, 93, 91);
			}
			else if (tiles[r][c] == GREEN)
			{
				drawTileGlow(
					tileX, tileY, tileW, tileH,
					5, 59, 32,
					12, 130, 71,
					72, 190, 131);
			}
		}
	}
}

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
		img);
}

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

	int leftX = 5;
	int leftY = 540;

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
		GLUT_BITMAP_8_BY_13);

	char greenCountsText[64];

	sprintf_s(
		greenCountsText,
		sizeof(greenCountsText),
		"YOU:%d    047:%d    392:%d",
		player.score,
		bots[0].score,
		bots[1].score);

	iSetColor(255, 255, 255);

	iTextCentered(
		leftCenterX,
		leftY + 12,
		greenCountsText,
		GLUT_BITMAP_8_BY_13);

	int rightX = 867;
	int rightY = 540;

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
		GLUT_BITMAP_8_BY_13);

	char timerText[16];

	sprintf_s(
		timerText,
		sizeof(timerText),
		"%d",
		level1Time);

	iSetColor(255, 255, 255);

	iTextCentered(
		rightCenterX,
		rightY + 10,
		timerText,
		GLUT_BITMAP_TIMES_ROMAN_24);
}

void drawPhasePrompt()
{
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
		GLUT_BITMAP_9_BY_15);
}

void drawEndScreen()
{
	bool won =
		(level1Result == RESULT_WIN_BOT_DIED) ||
		(level1Result == RESULT_WIN_TIME_UP);

	int img = gameOverImage;
	if (won)
	{
		img = (qualifiedBot == 0) ? victory1Image : victory2Image;
	}

	int dispW = won ? 620 : 560;
	int dispH = won ? (int)(dispW * 793.0 / 1983.0)
		: (int)(dispW * 941.0 / 1672.0);

	int x = (LEVEL1_WIDTH - dispW) / 2;
	int y = (LEVEL1_HEIGHT - dispH) / 2 + 20;

	iShowImage(x, y, dispW, dispH, img);

	int btnWidth = 220;
	int btnHeight = 48;
	int btnX = (LEVEL1_WIDTH - btnWidth) / 2;
	int btnY = y - btnHeight - 20;

	iSetColor(20, 20, 20);
	iFilledRectangle(btnX, btnY, btnWidth, btnHeight);

	iSetColor(255, 255, 255);
	iRectangle(btnX, btnY, btnWidth, btnHeight);

	iTextCentered(
		btnX + btnWidth / 2,
		btnY + btnHeight / 2 - 6,
		"< BACK TO LEVELS",
		GLUT_BITMAP_9_BY_15);

	backBtnX = btnX;
	backBtnY = btnY;
	backBtnW = btnWidth;
	backBtnH = btnHeight;
}

void drawLevel1()
{
	if (level1Phase == LEVEL1_PHASE_INTRO)
	{
		drawLevel1Intro();
		return;
	}

	if (level1Phase == LEVEL1_PHASE_COUNTDOWN)
	{
		drawLevel1Countdown();
		return;
	}

	iShowImage(
		0, 0,
		1000, 600,
		backgroundImage);

	drawTileGlow(
		GRID_X, GRID_Y,
		GRID_WIDTH, GRID_HEIGHT,
		15, 15, 15,
		28, 28, 28,
		45, 45, 45);

	drawTiles();

	drawCharacter(player);

	for (int i = 0; i < BOT_COUNT; i++)
	{
		drawCharacter(bots[i], i);
	}

	drawHUD();

	if (!level1GameOver)
	{
		drawPhasePrompt();
	}

	if (level1GameOver)
	{
		drawEndScreen();
	}
}

#endif