#ifndef AUDIO_HPP
#define AUDIO_HPP

void loadAudio()
{
	mciSendString("open \"Audios//menuMusic.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//l1music.mp3\" alias levelsong", NULL, 0, NULL);
	mciSendString("open \"Audios//startCountDown.mp3\" alias ggsong", NULL, 0, NULL);

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

void playMenuMusic()
{
	mciSendString("stop levelsong", NULL, 0, NULL);
	mciSendString("play bgsong repeat", NULL, 0, NULL);
}

#endif