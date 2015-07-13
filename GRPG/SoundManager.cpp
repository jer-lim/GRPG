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

	Music aidilPreFight;
	aidilPreFight.fileName = TEXT("assets/sound/aidil_prefight.wav");
	aidilPreFight.playTime = 29;
	aidilPreFight.id = soundManagerNS::aidilPreFightMusicID;
	musicList.push_back(aidilPreFight);

	Music aidilFight;
	aidilFight.fileName = TEXT("assets/sound/aidil_fight.wav");
	aidilFight.playTime = 126;
	aidilFight.id = soundManagerNS::aidilFightMusicID;
	musicList.push_back(aidilFight);

	Music firstWave;
	firstWave.fileName = TEXT("assets/sound/firstwave.wav");
	firstWave.playTime = 87;
	firstWave.id = soundManagerNS::firstWaveID;
	musicList.push_back(firstWave);

	Music secondwave;
	secondwave.fileName = TEXT("assets/sound/secondwave.wav");
	secondwave.playTime = 83;
	secondwave.id = soundManagerNS::secondWaveID;
	musicList.push_back(secondwave);

	Music thirdwave;
	thirdwave.fileName = TEXT("assets/sound/thirdwave.wav");
	thirdwave.playTime = 53;
	thirdwave.id = soundManagerNS::thirdWaveID;
	musicList.push_back(thirdwave);

	Music fourthwave;
	fourthwave.fileName = TEXT("assets/sound/fourthwave.wav");
	fourthwave.playTime = 53;
	fourthwave.id = soundManagerNS::fourthWaveID;
	musicList.push_back(fourthwave);

	Music fifthwave;
	fifthwave.fileName = TEXT("assets/sound/fifthwave.wav");
	fifthwave.playTime = 57;
	fifthwave.id = soundManagerNS::fifthWaveID;
	musicList.push_back(fifthwave);
}

void SoundManager::toggleMute()
{
	muted = !muted;
	if (muted)
	{
		stringstream ss;
		ss << "stop \"";
		ss << currentlyPlayingMusic.fileName;
		ss << "\"";
		mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
	}
	else
	{
		playMusic(currentlyPlayingMusic.id);
	}
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
	}
	currentlyPlayingMusic = musicList[musicID];
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

bool SoundManager::getMuted()
{
	return muted;
}