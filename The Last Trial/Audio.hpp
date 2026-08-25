#ifndef AUDIO_HPP
#define AUDIO_HPP

void loadAudio()
{
	mciSendString("open \"Audios//menuMusic.mp3\" alias bgsong", NULL, 0, NULL);
	mciSendString("open \"Audios//level1Music.mp3\" alias levelsong", NULL, 0, NULL);
	mciSendString("open \"Audios//gameover.mp3\" alias ggsong", NULL, 0, NULL);

	mciSendString("play bgsong repeat", NULL, 0, NULL);
}

void playLevel1Music()
{
	mciSendString("stop bgsong", NULL, 0, NULL);
	mciSendString("play levelsong repeat", NULL, 0, NULL);
}

#endif