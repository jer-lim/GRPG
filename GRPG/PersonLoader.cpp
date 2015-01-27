#include "PersonLoader.h"

void PersonLoader::loadAllNPCs()
{
	ifstream characterstream;
	//Friendly Characters
	characterstream.open(friendlynpcDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, descript;
		int id;
		float movementspd, attackcd;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd >> descript;
			NPC myChar = NPC(img_filename, movementspd, attackcd, name, descript, 9999);//almost infinite health
			map_friendlyNPCs[id] = myChar;
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
			//Enemy myChar = Enemy(img_filename, name, movementspd, attackcd);
			//map_friendlyNPCs[id] = myChar;
		}
		characterstream.close();
	}
}