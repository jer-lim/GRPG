#ifndef _RIFTDATA_H                 // Prevent multiple definitions if this 
#define _RIFTDATA_H                 // file is included in more than one place

#include "constants.h"
//#include "Character.h"
#include "NPC.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "string_functions.h"
#include <stdlib.h>     /* atoi */

struct RiftNPC
{
	//The npc id this data refers to
	int NPCId;
	//The difficulty of that NPC
	int difficulty;
	//The probability of the npc spawning
	int probability;
};

struct WaveDifficulty
{
	int minimumDifficulty;
	int maximumDifficulty;
	int xpReward;
};

class RiftData
{
private:
	const std::string riftDataLocation = "assets/npcs/rift_npcs.gdef";
	const std::string riftWaveDifficulty = "assets/npcs/rift_difficulty.gdef";
	vector<RiftNPC> rift_npcs;
	map<int, WaveDifficulty> rift_waves;
public:
	RiftData(){}
	void loadData();
	~RiftData(){
		rift_npcs.clear();
	}

	virtual vector<RiftNPC> getRiftNPC() { return rift_npcs; }

	//Returns a random difficulty number for a certain wave number.
	virtual int getRandomDifficulty(int waveNumber)
	{
		if (waveNumber < 0 || waveNumber > getMaximumWaves())
		{
			return 0;
		}
		else
		{
			WaveDifficulty wd = rift_waves[waveNumber];
			return ((rand() / RAND_MAX) * (wd.maximumDifficulty - wd.minimumDifficulty)) + wd.minimumDifficulty;
		}
	}

	virtual int getMaximumWaves() { return rift_waves.size() - 1; }

	virtual int getXPRewardForWave(int waveNumber)
	{
		return rift_waves[waveNumber].xpReward;
	}

};
#endif