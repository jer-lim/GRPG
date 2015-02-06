//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _PERSONLOADER_H                 // Prevent multiple definitions if this 
#define _PERSONLOADER_H                 // file is included in more than one place

#include "constants.h"
//#include "Character.h"
#include "NPC.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "string_functions.h"
#include <stdlib.h>     /* atoi */
#include "itemLoader.h"
#include "InventoryFood.h"
#include "InventoryEquipment.h"

class PersonLoader
{
private:
	const std::string friendlynpcDataLocation = "assets/npcs/friendly_npcs.gdef";
	const std::string enemyDataLocation = "assets/npcs/npcs.gdef";
	unordered_map<int, NPC*> map_npcs;
	//unordered_map<int, Enemy> map_enemies;
public:
	PersonLoader(){}
	void loadAllNPCs(ItemLoader* itemLoader);
	~PersonLoader(){
		for (unordered_map<int, NPC*>::iterator it = map_npcs.begin(); it != map_npcs.end(); ++it){
			SAFE_DELETE(it->second);
		}
		map_npcs.clear();
	}

	//Make sure to use only the IDs for these 2 or else it is likely that an exception is thrown
	//This is a design choice
	/*NPC* getFriendlyNPC(int id){
		return &map_friendlyNPCs[id];
	}
	Enemy* getEnemy(int id){
		return &map_enemies[id];
	}*/
	NPC* getNPC(int id){
		return map_npcs[id];
	}
};
#endif