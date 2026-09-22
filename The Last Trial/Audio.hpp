#ifndef AUDIO_HPP
#define AUDIO_HPP

void loadAudio()
{
	mciSendString("open \"Audios//menuMusic.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//l1music.mp3\" alias levelsong", NULL, 0, NULL);
	mciSendString("open \"Audios//startCountDown.mp3\" alias ggsong", NULL, 0, NULL);
	mciSendString("open \"Audios//bgmusic_level_2.mp3\" alias level2song", NULL, 0, NULL);   
	mciSendString("open \"Audios//bgmusic_level_3.mp3\" alias level3song", NULL, 0, NULL);   

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
	mciSendString("stop level2song", NULL, 0, NULL);   // NEW
	mciSendString("stop level3song", NULL, 0, NULL);   // NEW
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

#endif