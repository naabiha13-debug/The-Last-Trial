#ifndef SAVEDATA_HPP
#define SAVEDATA_HPP

#include <stdio.h>
#include <string.h>

#define SAVE_FILE "savedata.txt"

#define SAVE_NOT_PLAYED 0
#define SAVE_WON 1
#define SAVE_LOST 2

int saveCurrentLevel = 1;
int saveHighestUnlocked = 1;// player kon level porjonto pouchechhe
int saveL1Player = 0;            // Level 1 player score
int saveL1Bot1 = 0;              // Level 1 bot 1 score
int saveL1Bot2 = 0;              // Level 1 bot 2 score
int saveL1QualBot = 0;           // 0 = keu na, 1 = Bot1, 2 = Bot2
int saveL2Result = SAVE_NOT_PLAYED;
int saveL3Result = SAVE_NOT_PLAYED;

const char* saveResultToText(int r)
{
	if (r == SAVE_WON) return "WON";
	if (r == SAVE_LOST) return "LOST";
	return "NOTPLAYED";
}

int saveTextToResult(const char* s)
{
	if (strcmp(s, "WON") == 0) return SAVE_WON;
	if (strcmp(s, "LOST") == 0) return SAVE_LOST;
	return SAVE_NOT_PLAYED;
}

const char* saveBotToText(int b)
{
	if (b == 1) return "Bot1";
	if (b == 2) return "Bot2";
	return "NONE";
}

int saveTextToBot(const char* s)
{
	if (strcmp(s, "Bot1") == 0) return 1;
	if (strcmp(s, "Bot2") == 0) return 2;
	return 0;
}

void resetSaveData()
{
	saveCurrentLevel = 1;
	saveHighestUnlocked = 1;
	saveL1Player = saveL1Bot1 = saveL1Bot2 = 0;
	saveL1QualBot = 0;
	saveL2Result = SAVE_NOT_PLAYED;
	saveL3Result = SAVE_NOT_PLAYED;
}

void saveGame()
{
	FILE *fp = fopen(SAVE_FILE, "w");
	if (fp == NULL) return;
	fprintf(fp, "CurrentLevel: %d\n", saveCurrentLevel);
	fprintf(fp, "HighestUnlocked: %d\n", saveHighestUnlocked);
	fprintf(fp, "Level1_Player: %d\n", saveL1Player);
	fprintf(fp, "Level1_Bot1: %d\n", saveL1Bot1);
	fprintf(fp, "Level1_Bot2: %d\n", saveL1Bot2);
	fprintf(fp, "Level1_QualifiedBot: %s\n", saveBotToText(saveL1QualBot));
	fprintf(fp, "Level2_Opponent: %s\n", saveBotToText(saveL1QualBot));
	fprintf(fp, "Level2_Result: %s\n", saveResultToText(saveL2Result));
	fprintf(fp, "Level3_Result: %s\n", saveResultToText(saveL3Result));
	fclose(fp);
}

void loadGame()
{
	FILE *fp = fopen(SAVE_FILE, "r");
	if (fp == NULL)
	{
		resetSaveData();
		saveGame();      // first time hole notun file banay dey
		return;
	}

	char qb[20], opp[20], r2[20], r3[20];
	int ok = fscanf(fp,
		" CurrentLevel: %d HighestUnlocked: %d Level1_Player: %d Level1_Bot1: %d Level1_Bot2: %d Level1_QualifiedBot: %19s Level2_Opponent: %19s Level2_Result: %19s Level3_Result: %19s",
		&saveCurrentLevel, &saveHighestUnlocked, &saveL1Player, &saveL1Bot1, &saveL1Bot2, qb, opp, r2, r3);
	fclose(fp);

	if (ok != 9)
	{
		resetSaveData();   // file kharap ba purano format hole reset
		saveGame();
		return;
	}
	saveL1QualBot = saveTextToBot(qb);
	saveL2Result = saveTextToResult(r2);
	saveL3Result = saveTextToResult(r3);
}

// Level 1 sesh hole call korbe.
// qualifiedBot: 1 = Bot1 jitse, 2 = Bot2 jitse, 0 = keu na
// playerPassed: 1 jodi player Level 2 te jay, nahole 0
void saveLevel1Result(int playerScore, int bot1Score, int bot2Score, int qualifiedBot, int playerPassed)
{
	saveL1Player = playerScore;
	saveL1Bot1 = bot1Score;
	saveL1Bot2 = bot2Score;
	saveL1QualBot = qualifiedBot;
	if (playerPassed && saveHighestUnlocked < 2) saveHighestUnlocked = 2;
	saveGame();
}

// Level 2 sesh hole call korbe (won = 1 jodi player jite, nahole 0)
void saveLevel2Result(int won)
{
	saveL2Result = won ? SAVE_WON : SAVE_LOST;
	if (won && saveHighestUnlocked < 3) saveHighestUnlocked = 3;
	saveGame();
}

// Level 3 sesh hole call korbe (won = 1 jodi player jite, nahole 0)
void saveLevel3Result(int won)
{
	saveL3Result = won ? SAVE_WON : SAVE_LOST;
	saveGame();
}


void setCurrentLevel(int lvl)
{
	saveCurrentLevel = lvl;
	saveGame();
}

#endif