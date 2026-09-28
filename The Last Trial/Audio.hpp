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
	mciSendString("open \"Audios//game_over.mpeg\" alias overSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//jump.mpeg\" alias jumpSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//door.mp3\" alias doorSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//waterstream.mp3\" alias waterStreamSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//watersplash.mp3\" alias waterSplashSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//equipment_collect.mp3\" alias equipSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//glasshint.mp3\" alias glassHintSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//pin.mpeg\" alias pinSfx", NULL, 0, NULL);
	mciSendString("open \"Audios//unlocked_pin.mp3\" alias unlockedPinSfx", NULL, 0, NULL);  
	mciSendString("open \"Audios//error_pin.mp3\" alias errorPinSfx", NULL, 0, NULL);       
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
	mciSendString("play overSfx from 0", NULL, 0, NULL);
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

void stopCountdownMusic()
{
	mciSendString("stop ggsong", NULL, 0, NULL);
}

void playJumpSound()
{
	mciSendString("stop jumpSfx", NULL, 0, NULL);
	mciSendString("play jumpSfx from 0", NULL, 0, NULL);
}

void playDoorSound()
{
	mciSendString("stop doorSfx", NULL, 0, NULL);
	mciSendString("play doorSfx from 0", NULL, 0, NULL);
}

void playWaterSplashSound()
{
	mciSendString("stop waterSplashSfx", NULL, 0, NULL);
	mciSendString("play waterSplashSfx from 0", NULL, 0, NULL);
}

void playEquipmentCollectSound()
{
	mciSendString("stop equipSfx", NULL, 0, NULL);
	mciSendString("play equipSfx from 0", NULL, 0, NULL);
}

void playGlassHintSound()
{
	mciSendString("stop glassHintSfx", NULL, 0, NULL);
	mciSendString("play glassHintSfx from 0", NULL, 0, NULL);
}

void playPinSound()
{
	mciSendString("stop pinSfx", NULL, 0, NULL);
	mciSendString("play pinSfx from 0", NULL, 0, NULL);
}

void playUnlockedPinSound()
{
	mciSendString("stop unlockedPinSfx", NULL, 0, NULL);
	mciSendString("play unlockedPinSfx from 0", NULL, 0, NULL);
}

void playErrorPinSound()
{
	mciSendString("stop errorPinSfx", NULL, 0, NULL);
	mciSendString("play errorPinSfx from 0", NULL, 0, NULL);
}

// river er kache thakle loop e bajbe, dure gele bondho hobe
bool waterStreamPlaying = false;
void updateWaterStreamSound(bool shouldPlay)
{
	if (shouldPlay && !waterStreamPlaying)
	{
		mciSendString("play waterStreamSfx from 0 repeat", NULL, 0, NULL);
		waterStreamPlaying = true;
	}
	else if (!shouldPlay && waterStreamPlaying)
	{
		mciSendString("stop waterStreamSfx", NULL, 0, NULL);
		waterStreamPlaying = false;
	}
}
#endif