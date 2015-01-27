#include "PersonLoader.h"

void PersonLoader::loadAllNPCs()
{
	ifstream characterstream;
	//Friendly Characters
	characterstream.open(friendlynpcDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description;
		int id;
		float movementspd, attackcd;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd >> description;
			NPC myChar = NPC(img_filename, movementspd, attackcd, name, description, 9999);//almost infinite health
			map_npcs[id] = myChar;
		}
		characterstream.close();
	}
	//All Enemies
	characterstream.open(enemyDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description;
		int id, atk, str, def, hp;
		float movementspd, attackcd, dmg_red;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd
				>> description >> atk >> str >> def >> hp >> dmg_red;
			Enemy myChar = Enemy(img_filename, movementspd, attackcd, name, description, hp, atk, str, def, dmg_red);
			map_npcs[id] = myChar;
		}
		characterstream.close();
	}
}