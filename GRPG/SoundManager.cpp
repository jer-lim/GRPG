#include "NoiseManager.h"
#include <thread>

vector<Music> SoundManager::musicList = vector<Music>();
bool SoundManager::muted = false;
Music SoundManager::currentlyPlayingMusic = Music();
float SoundManager::musicTimer = 0;

void SoundManager::initialize()
{
	musicList = vector<Music>();
	Music easyLemon;
	easyLemon.fileName = TEXT("assets/sound/easylemon.wav");
	easyLemon.playTime = 126;
	easyLemon.id = soundManagerNS::generalMusicID;
	musicList.push_back(easyLemon);

	Music movementProposition;
	movementProposition.fileName = TEXT("assets/sound/movementproposition.wav");
	movementProposition.playTime = 141;
	movementProposition.id = soundManagerNS::fightingMusicID;
	musicList.push_back(movementProposition);
}

void SoundManager::mute()
{
	muted = true;
	stringstream ss;
	ss << "stop \"";
	ss << currentlyPlayingMusic.fileName;
	ss << "\"";
	mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
	currentlyPlayingMusic = Music();
}

void SoundManager::update(float frametime, Game* gamePtr)
{
	if (!muted)
	{
		musicTimer -= frametime;
		if (musicTimer < 0)
		{
			playMusic(currentlyPlayingMusic.id);
		}
	}
}

void SoundManager::playMusic(int musicID)
{
	if (!muted)
	{
		int errorCode;
		stringstream ss;
		//Stop currently playing music
		if (currentlyPlayingMusic.fileName != "")
		{
			ss << "stop \"";
			ss << currentlyPlayingMusic.fileName;
			ss << "\""; 
			errorCode = mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
			ss.str("");
		}
		//Search for music
		ss << "play \"";
		ss << musicList[musicID].fileName;
		ss << "\"";
		musicTimer = musicList[musicID].playTime;
		errorCode = mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
		currentlyPlayingMusic = musicList[musicID];
		int a = errorCode;
	}
}

void SoundManager::playSound(const char soundName[])
{
	if (!muted)
	{
		//Search for sound
		stringstream ss;
		ss << "play \"";
		ss << soundName;
		ss << "\"";
		int errorCode = mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
	}
}

int SoundManager::getcurrentMusicId()
{
	return currentlyPlayingMusic.id;
}