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

void initMenu()
{
	menuBackground = iLoadImage("Image//bg.png");
	menuTitle = iLoadImage("Image//title.png");
	menuCharacter = iLoadImage("Image//character.png");
	buttonPlay = iLoadImage("Image//play.png");
	buttonStory = iLoadImage("Image//story.png");
	buttonCredit = iLoadImage("Image//credits.png");
	buttonExit = iLoadImage("Image//exit.png");
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
}

#endif