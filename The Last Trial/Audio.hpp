#ifndef AUDIO_HPP
#define AUDIO_HPP

void loadAudio()
{
	mciSendString("open \"Audios//menuMusic.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//l1music.mp3\" alias levelsong", NULL, 0, NULL);
	mciSendString("open \"Audios//startCountDown.mp3\" alias ggsong", NULL, 0, NULL);
	mciSendString("open \"Audios//bgmusic_level_2.mp3\" alias level2song", NULL, 0, NULL);   
	mciSendString("open \"Audios//bgmusic_level_3.mp3\" alias level3song", NULL, 0, NULL);   
	mciSendString("open \"Audios//game_win.mpeg\" alias winSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//game_over.mp3\" alias overSfx", NULL, 0, NULL);
	mciSendString("set overSfx time format milliseconds", NULL, 0, NULL);
	mciSendString("open \"Audios//axe_sound.mpeg\" alias axeSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//gun_sound.mpeg\" alias gunSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//gun_sound.mpeg\" alias gunSfxEnemy", NULL, 0, NULL);
	mciSendString("open \"Audios//biscuit_collect.mpeg\" alias biscuitSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//key_collect.mpeg\" alias keySfx", NULL, 0, NULL);
	mciSendString("open \"Audios//paperhint_collect.mpeg\" alias paperSfx", NULL, 0, NULL);
    mciSendString("play bgsong repeat", NULL, 0, NULL);
}

void stopMenuMusic()
{
	mciSendString("stop bgsong", NULL, 0, NULL);
}

void playCountdownMusic()
{
	mciSendString("stop bgsong", NULL, 0, NULL);
	mciSendString("play ggsong from 0", NULL, 0, NULL);
}

void playLevel1Music()
{
	mciSendString("stop ggsong", NULL, 0, NULL);
	mciSendString("stop bgsong", NULL, 0, NULL);
	mciSendString("play levelsong repeat", NULL, 0, NULL);
}

void playLevel2Music()
{
	mciSendString("stop bgsong", NULL, 0, NULL);
	mciSendString("play level2song from 0 repeat", NULL, 0, NULL);
}

void playLevel3Music()
{
	mciSendString("stop bgsong", NULL, 0, NULL);
	mciSendString("play level3song from 0 repeat", NULL, 0, NULL);
}

void playMenuMusic()
{
	mciSendString("stop levelsong", NULL, 0, NULL);
	mciSendString("stop level2song", NULL, 0, NULL);   
	mciSendString("stop level3song", NULL, 0, NULL);   
	mciSendString("stop overSfx", NULL, 0, NULL);
	mciSendString("stop winSfx", NULL, 0, NULL);
	mciSendString("play bgsong repeat", NULL, 0, NULL);
}

void stopLevel1Music()
{
	mciSendString("stop levelsong", NULL, 0, NULL);
}

void stopLevel2Music()
{
	mciSendString("stop level2song", NULL, 0, NULL);
}

void stopLevel3Music()
{
	mciSendString("stop level3song", NULL, 0, NULL);
}

void playGameWinSound()
{
	mciSendString("stop winSfx", NULL, 0, NULL);
	mciSendString("play winSfx from 0", NULL, 0, NULL);
}

void playGameOverSound()
{
	mciSendString("stop overSfx", NULL, 0, NULL);
	mciSendString("play overSfx from 1100 to 3100", NULL, 0, NULL);
}

void playAxeSound()
{
	mciSendString("stop axeSfx", NULL, 0, NULL);
	mciSendString("play axeSfx from 0", NULL, 0, NULL);
}

void playGunSound()
{
	mciSendString("stop gunSfx", NULL, 0, NULL);
	mciSendString("play gunSfx from 0", NULL, 0, NULL);
}

void stopGunSound()
{
	mciSendString("stop gunSfx", NULL, 0, NULL);
}

void playEnemyGunSound()
{
	mciSendString("stop gunSfxEnemy", NULL, 0, NULL);
	mciSendString("play gunSfxEnemy from 0", NULL, 0, NULL);
}

void stopEnemyGunSound()
{
	mciSendString("stop gunSfxEnemy", NULL, 0, NULL);
}
void playBiscuitCollectSound()
{
	mciSendString("stop biscuitSfx", NULL, 0, NULL);
	mciSendString("play biscuitSfx from 0", NULL, 0, NULL);
}

void playKeyCollectSound()
{
	mciSendString("stop keySfx", NULL, 0, NULL);
	mciSendString("play keySfx from 0", NULL, 0, NULL);
}

void playPaperHintCollectSound()
{
	mciSendString("stop paperSfx", NULL, 0, NULL);
	mciSendString("play paperSfx from 0", NULL, 0, NULL);
}

#endif