#ifndef MENU_HPP
#define MENU_HPP

int menuBackground;
int menuTitle;
int menuScreen = 0;
int menuCharacter;
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


void initMenu()
{
	menuBackground = iLoadImage("Image//bg.png");
	menuTitle = iLoadImage("Image//title.png");
	menuCharacter = iLoadImage("Image//character.png");
	buttonPlay = iLoadImage("Image//play.png");
	buttonStory = iLoadImage("Image//story.png");
	buttonCredit = iLoadImage("Image//credits.png");
	buttonExit = iLoadImage("Image//exit.png");
	buttonLevel1 = iLoadImage("Image//level1.png");
	buttonLevel2 = iLoadImage("Image//level2.png");
	buttonLevel3 = iLoadImage("Image//level3.png");
	creditsImage = iLoadImage("Image//team.png");
}
void DrawMenu()
{
	iShowImage(0, 0, 1000, 600, menuBackground);

	if (menuScreen == 0)
	{
		iShowImage(200, 300, 600, 200, menuTitle);
	}
	else if (menuScreen == 1)
	{
		iShowImage(540, 5, 300, 460, menuCharacter);

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
}
void DrawCredits()
{
	iShowImage(0, 0, 1000, 600, menuBackground);
	iShowImage(200, 100, 600, 400, creditsImage);   // was buttonCredit
}


#endif