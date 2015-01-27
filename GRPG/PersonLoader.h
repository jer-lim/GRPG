#ifndef _PERSONLOADER_H                 // Prevent multiple definitions if this 
#define _PERSONLOADER_H                 // file is included in more than one place

#include "constants.h"
//#include "Character.h"
//#include "NPC.h"
#include "Enemy.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class PersonLoader
{
private:
	const std::string friendlynpcDataLocation = "assets/npcs/friendly_npcs.gdef";
	const std::string enemyDataLocation = "assets/npcs/npcs.gdef";
	unordered_map<int, NPC> map_friendlyNPCs;
	unordered_map<int, Enemy> map_enemies;
public:
	void loadAllNPCs();
};
#endif