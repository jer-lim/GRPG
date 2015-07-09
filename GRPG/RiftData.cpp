#include "RiftData.h"
#include "Rift.h"
#include <algorithm>

void RiftData::loadData()
{
	ifstream characterstream;
	//Friendly Characters
	characterstream.open(riftDataLocation);
	if (characterstream.is_open()){
		int id;
		float d, p;
		while (!characterstream.eof()){
			characterstream >> id >> d >> p;

			RiftNPC rn;
			rn.NPCId = id;
			rn.difficulty = d;
			rn.probability = p;

			rift_npcs.push_back(rn);
		}
		characterstream.close();
	}

	characterstream.open(riftWaveDifficulty);
	if (characterstream.is_open()){
		int wave, min, max, reward, musicId;
		string soundFile;
		while (!characterstream.eof()){
			characterstream >> wave >> min >> max >> reward >> musicId >> soundFile;

			WaveDifficulty wd;
			wd.maximumDifficulty = max;
			wd.minimumDifficulty = min;
			wd.xpReward = reward;
			wd.musicId = musicId;
			wd.soundFilePath = soundFile;

			rift_waves[wave] = wd;
		}
		characterstream.close();
	}
}