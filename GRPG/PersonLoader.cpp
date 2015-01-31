#include "PersonLoader.h"
#include "Enemy.h"
#include "NPC.h"
#include <algorithm>

void PersonLoader::loadAllNPCs()
{

	ifstream characterstream;
	//Friendly Characters
	characterstream.open(friendlynpcDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description;
		int id, cols;
		float movementspd, attackcd, height, width, colHeight, colWidth;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd >> height >> width >> cols >> colHeight >> colWidth >> description;
			std::replace(name.begin(), name.end(), '_', ' ');
			std::replace(description.begin(), description.end(), '_', ' ');
			NPC* myChar = new NPC(img_filename, movementspd, attackcd, height, width, cols, colHeight, colWidth, name, description, 9999, false);//almost infinite health
			map_npcs[id] = myChar;
		}
		characterstream.close();
	}
	//All Enemies
	characterstream.open(enemyDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description;
		int id, atk, str, def, hp, cols, aggro;
		float movementspd, attackcd, dmg_red, width, height, colHeight, colWidth;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd>> height >> width >> cols >> colHeight >> colWidth
				>>  description >> atk >> str >> def >> hp >> dmg_red >> aggro;
			std::replace(name.begin(), name.end(), '_', ' ');
			std::replace(description.begin(), description.end(), '_', ' ');
			Enemy* myChar = new Enemy(img_filename, movementspd, attackcd, height, width, cols, colHeight, colWidth, name, description, hp, atk, str, def, dmg_red, aggro);
			map_npcs[id] = myChar;
		}
		characterstream.close();
	}
}