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
	const char aidilHitSoundFileName[] = "assets/sound/rocket.wav";
	const char hitSoundFileName[] = "assets/sound/sword_v_sword.wav";
	const char levelUpSoundFileName[] = "assets/sound/levelup.wav";

	const int generalMusicID = 0;
	const int fightingMusicID = 1;
	const int aidilPreFightMusicID = 2;
	const int aidilFightMusicID = 3;
	const int firstWaveID = 4;
	const int secondWaveID = 5;
	const int thirdWaveID = 6;
	const int fourthWaveID = 7;
	const int fifthWaveID = 8;
}

struct Music
{
	int id;
	string fileName;
	int playTime;
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
// Ok, so mciSendString has a few problems. After reading the docs, I thought of doing the following:
// 1) Initialize ALL sounds and music at the start using the initialize function, calling the open command
// for all of them. For simplicity, they can all be aliased to a name.
// 2) When a sound is needed to be played, simply play it
// 3) When a music is needed to be played, save it into the currentlyPlayedMusic, and play it on repeat.
// If the program is ever muted or the music changed, call the stop command for that music.
// 4) At the end,c all close on everything.
// This has a few problems. Namely, the most important one being is that ALL sound and music is only ever
// played once. For example, once the game plays the "attack" sound, the sound is played. The next time another
// call is performed, NOTHING is played. But, if you don't bother calling the open thing and just instead send
// the command to play the file name, it always works (I wonder why I should bother pre-opening it then, that seems
// so useless.) Another problem is that the repeat command doesn't work, but that's sort of OK, we can handle that with the
// update function I used for PlaySound to handle music.
// So, after Googling to find out why I can't pre-initalize a sound file and play it multiple times, I came across a thread
// that suggested that I needed to call "stop" on the sound before I can replay it again. Which seems sensible. But I obviously
// can't call stop immediately afterwards, only after the sound has finished playing. I could use the wait command to pause the
// thread until the sound finishes, but I can't possibly do that for the main thread. The solution? Whenever a call is required,
// Spin up a new thread that will execute two commands. One, play sound, waiting until it is finished. Two, stop playing the sound
// Sounds good. Unfortuntely, it doesn't work, but commands returning error 263 - MCIERR_INVALID_DEVICE_NAME.
// After a lot more testing, it turns out it doesn't work because OPENED sounds only work on the SAME THREAD they were opened,
// NOT on others. Before I started implementing a producer/consumer pattern, I decided to test to see if this PLAY / WAIT / STOP
// thing even worked for playing the sound more than once, by running it on the main thread.
// Turns out, NO IT DOESN'T. The sound will only play once, then no longer play. Both PLAY and STOP commands will return an error
// code of 0. I give up. From now on, all sounds/music will just be played with loading the sound file whenever it's required
// instead of preloading it for all users. AT LEAST THAT ONE WORKS!
// Also I'm converting all mp3 files to wav files despite their significantly larger file size because mcisendstring doesn't
// play mp3 files. The error code returned if I try is 277, which is MCIERR_INTERNAL... Wow.
class SoundManager {
	static bool muted;
	static vector<Music> musicList;
	static Music currentlyPlayingMusic;
	static float musicTimer;
public:
	static void initialize();

	static void toggleMute();

	static void update(float frametime, Game* gamePtr);

	static void playMusic(int musicID);

	static void playSound(const char soundName[]);

	static int getcurrentMusicId();

	static bool getMuted();
};

#endif