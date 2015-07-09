// Module: Games Programming
// Assignment 2: GRPG
// Student Name: Jeremy Lim
// Student Number: S10122326F

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "game.h"
#include <sstream>
#include <string>
#include <winerror.h>

namespace soundManagerNS {
	const int hitSoundID = 0;

	const int easyLemonMusicID = 0;
}

struct Noise
{
	int id;
	string name;
	string fileName;
};

struct Music : Noise
{
};

struct Sound : Noise
{
};

//////////////////////////////////////////////////////////////////////////////////////////
// I tried other attempts at finding a (more) suitable way to handle sound - PlaySound seems
//like a primitive way that might work for now but will likely need to change to a better one soo.
//Attempted to search for a solution:
//https://msdn.microsoft.com/en-us/library/windows/desktop/ee415813%28v=vs.85%29.aspx - XAudio2 Introduction
// XAudio2 introduces itself as long-awaited replacement for DirectSound. It addresses several outstanding issues and feature requests.
// FAILED - The game kept on producing errors at the end for unknown reasons, and the error line was always at some
// location within external code, and the entire call stack was external code meaning that I could not find
// out where the error was. Despite following the docs, did not find any way to solve this problem.
// Gave up. Would love to get it working, as it seems like it allows good commands such as 
// allowing the ability to hook listeners to music and sounds to listen for when they finish, as well
// as stopping them in the middle of playing them. 
// http://www.sfml-dev.org - SFML - Simple and Fast multimedia Library.
// Advertises itself as easy to use and multi-platform. Requires the import of a new library.
// FAILED - Had plenty of linker errors despite following the guide and dependies. Additionally, not sure if it 
// would work when the game is packaged up - might have a few unforseen errors. Also not sure if I can get it
// working with git, given that the docs suggest placing the files not in the working directory and instead have an
// absolute file path in the configuration settings - sure, it might work in my computer with THAT absolute file path
// but what about the others?
// In the end, settled with using Jeremy's old PlaySound code. Hey, if it works...
// Turns out PlaySound cannot play simultaenous sounds. Changing to mciSendString
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd757161%28v=vs.85%29.aspx
class SoundManager {
	static bool muted;
	static vector<Music> musicList;
	static Music currentlyPlayingMusic;
	static vector<Sound> soundList;

public:
	static void initialize()
	{
		musicList = vector<Music>();
		Music m;
		m.fileName = TEXT("assets/sound/easylemon.wav");
		m.name = "easylemon";
		//m.playTime = 126;
		m.id = soundManagerNS::easyLemonMusicID;
		musicList.push_back(m);
		//Load all sounds
		soundList = vector<Sound>();
		Sound s;
		s.fileName = TEXT("assets/sound/rocket.wav");
		s.name = "attack";
		s.id = soundManagerNS::hitSoundID;
		soundList.push_back(s);

		vector<Noise> noiseList;
		noiseList.reserve(soundList.size() + musicList.size());
		noiseList.insert(noiseList.end(), soundList.begin(), soundList.end());
		noiseList.insert(noiseList.end(), musicList.begin(), musicList.end());

		stringstream ss;
		for (vector<Noise>::iterator i = noiseList.begin(); i != noiseList.begin(); i++)
		{
			Noise theNoise = *i;
			ss << "open ";
			ss << theNoise.fileName;
			ss << " alias ";
			ss << theNoise.name;
			mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
			ss.str("");
		}
	}

	static void mute()
	{
		muted = true;
		stringstream ss;
		ss << "stop ";
		ss << currentlyPlayingMusic.name;
		mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
	}

	static void update(float frametime, Game* gamePtr)
	{
		/*		if (!muted)
		{
			musicTimer -= frametime;
			if (musicTimer < 0)
			{
				playMusic(currentlyPlayingMusic.id);
			}
		}*/
	}

	static void playMusic(int musicID)
	{
		if (!muted)
		{
			//Stop currently playing music
			stringstream ss;
			ss << "stop ";
			ss << currentlyPlayingMusic.name;
			mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
			ss.str("");
			//Search for sound
			ss << "play ";
			ss << musicList[musicID].name;
			ss << " repeat";
			mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
		}
	}

	static void playSound(int soundID){
		if (!muted)
		{
			//Search for sound
			stringstream ss;
			ss << "play ";
			ss << soundList[soundID].name;
			ss << " wait";
			mciSendString(TEXT(ss.str().c_str()), NULL, 0, NULL);
		}
	}
};

#endif