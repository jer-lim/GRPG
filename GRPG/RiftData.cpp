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
		int wave, min, max;
		while (!characterstream.eof()){
			characterstream >> wave >> min >> max;

			WaveDifficulty wd;
			wd.maximumDifficulty = max;
			wd.minimumDifficulty = min;

			rift_waves[wave] = wd;
		}
		characterstream.close();
	}
}