#ifndef MENU_HPP
#define MENU_HPP

int menuBackground;
int menuTitle;
int menuScreen = 0;
int buttonPlay;
int buttonStory;
int buttonCredit;
int buttonExit;
int hoverButton = 0;
int selectedLevel = 0;
int buttonLevel1;
int buttonLevel2;
int buttonLevel3;
int hoverLevel = 0;
int creditsImage;
int nextButton;
int backButton;
int story1;
int story2;
int characterNormal;
int characterSlightLeft;
int characterLeft;
int characterSlightRight;
int characterRight;

int characterFrame = 0;
int characterTimer = 0;
int enterImage;

bool hoverBack = false;




void initMenu()
{
	menuBackground = iLoadImage("Image//bg.png");
	menuTitle = iLoadImage("Image//title.png");
	characterNormal = iLoadImage("Image//normal.png");
	characterLeft = iLoadImage("Image//left.png");
	characterRight = iLoadImage("Image//right.png");
	buttonPlay = iLoadImage("Image//play.png");
	buttonStory = iLoadImage("Image//story.png");
	buttonCredit = iLoadImage("Image//credits.png");
	buttonExit = iLoadImage("Image//exit.png");
	buttonLevel1 = iLoadImage("Image//level1.png");
	buttonLevel2 = iLoadImage("Image//level2.png");
	buttonLevel3 = iLoadImage("Image//level3.png");
	creditsImage = iLoadImage("Image//creditnames.png");
	nextButton = iLoadImage("Image//next.png");
	backButton = iLoadImage("Image//back.png");
	story1 = iLoadImage("Image//story1.png");
	story2 = iLoadImage("Image//story2.png");
	characterNormal = iLoadImage("Image//hn.png");
	characterSlightLeft = iLoadImage("Image//hml.png");
	characterLeft = iLoadImage("Image//hl.png");
	characterSlightRight = iLoadImage("Image//hmr.png");
	characterRight = iLoadImage("Image//hr.png");
	enterImage = iLoadImage("Image//enter.png");
}
void DrawMenu()
{
	iShowImage(0, 0, 1000, 600, menuBackground);

	if (menuScreen == 0)
	{
		iShowImage(200, 300, 600, 200, menuTitle);
		iShowImage(760, 75, 150, 50, enterImage);
	}
	else if (menuScreen == 1)
	{
		if (characterFrame == 0)
		{
			iShowImage(540, 10, 290, 457, characterNormal);
		}
		else if (characterFrame == 1)
		{
			iShowImage(540, 10, 290, 457, characterSlightLeft);
		}
		else if (characterFrame == 2)
		{
			iShowImage(540, 10, 290, 457, characterLeft);
		}
		else if (characterFrame == 3)
		{
			iShowImage(540, 10, 290, 457, characterSlightRight);
		}
		else if (characterFrame == 4)
		{
			iShowImage(540, 10, 290, 457, characterRight);
		}
		if (hoverButton == 1)
			iShowImage(125, 345, 280, 90, buttonPlay);
		else
			iShowImage(130, 350, 270, 80, buttonPlay);

		if (hoverButton == 2)
			iShowImage(125, 265, 280, 90, buttonStory);
		else
			iShowImage(130, 270, 270, 80, buttonStory);

		if (hoverButton == 3)
			iShowImage(125, 185, 280, 90, buttonCredit);
		else
			iShowImage(130, 190, 270, 80, buttonCredit);

		if (hoverButton == 4)
			iShowImage(125, 105, 280, 90, buttonExit);
		else
			iShowImage(130, 110, 270, 80, buttonExit);
	}
	else if (menuScreen == 2)
	{
		iShowImage(130, 350, 270, 80, buttonLevel1);
		iShowImage(130, 250, 270, 80, buttonLevel2);
		iShowImage(130, 150, 270, 80, buttonLevel3);
	}
	
}
void DrawLevelSelection()
{
	iShowImage(0, 0, 1000, 600, menuBackground);

	if (hoverLevel == 1)
		iShowImage(360, 355, 280, 90, buttonLevel1);
	else
		iShowImage(365, 360, 270, 80, buttonLevel1);

	if (hoverLevel == 2)
		iShowImage(360, 255, 280, 90, buttonLevel2);
	else
		iShowImage(365, 260, 270, 80, buttonLevel2);

	if (hoverLevel == 3)
		iShowImage(360, 155, 280, 90, buttonLevel3);
	else
		iShowImage(365, 160, 270, 80, buttonLevel3);

	if (hoverLevel == 3)
		iShowImage(360, 155, 280, 90, buttonLevel3);
	else
		iShowImage(365, 160, 270, 80, buttonLevel3);

	iShowImage(60, 500, 140, 50, backButton);
}
void DrawCredits()
{
	iShowImage(0, 0, 1000, 600, menuBackground);
	iShowImage(200, 100, 600, 400, creditsImage);

	// BACK button
	iShowImage(60, 500, 140, 50, backButton);
}
/*void updateCharacterAnimation()
{
	characterTimer++;

	if (characterTimer >= 8)
	{
		characterTimer = 0;

		characterFrame++;

		if (characterFrame > 8)
			characterFrame = 0;
	}
}*/
void updateCharacterAnimation()
{
	characterTimer++;

	if (characterTimer >= 8)
	{
		characterTimer = 0;

		characterFrame++;

		if (characterFrame > 4)
		{
			characterFrame = 0;
		}
	}
}

void DrawStory()
{
	if (storyPage == 1)
	{
		iShowImage(0, 0, 1000, 600, story1);

		// NEXT button
		iShowImage(800, 500, 140, 50, nextButton);
	}
	else if (storyPage == 2)
	{
		iShowImage(0, 0, 1000, 600, story2);

		// BACK button
		iShowImage(60, 500, 140, 50, backButton);
	}
}


#endif