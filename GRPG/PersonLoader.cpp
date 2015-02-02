#include "PersonLoader.h"
#include "Enemy.h"
#include "NPC.h"
#include <algorithm>

void PersonLoader::loadAllNPCs(ItemLoader* itemLoader)
{

	ifstream characterstream;
	//Friendly Characters
	characterstream.open(friendlynpcDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description, talk, dropsData;
		int id, cols;
		float movementspd, attackcd, height, width, colHeight, colWidth;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd >> height >> width >> cols >> colHeight >> colWidth >> description >> talk >> dropsData;
			std::replace(name.begin(), name.end(), '_', ' ');
			std::replace(description.begin(), description.end(), '_', ' ');
			std::replace(talk.begin(), talk.end(), '_', ' ');

			//get shop_items data

			vector<InventoryItem*>* dropsList = nullptr;

			dropsData.erase(dropsData.begin());
			dropsData.erase(dropsData.end() - 1);
			if (dropsData.size() > 0)
			{
				dropsList = new vector<InventoryItem*>();
				InventoryItem* iiPtr;
				vector<string> vector_dropsData = String_Functions::split(dropsData, ',');
				for (int i = 0, l = vector_dropsData.size(); i < l; ++i)
				{
					vector<string> vector_drop = String_Functions::split(vector_dropsData[i], ':');
					if (vector_drop[0].at(0) == 'F')
					{//Food "id:count:food_state"
						vector_drop[0].erase(0);
						iiPtr = new InventoryFood(itemLoader->getItem(atoi(vector_drop[0].c_str())), atoi(vector_drop[1].c_str()),
							atoi(vector_drop[2].c_str()));
					}
					else if (vector_drop[0].at(0) == 'E')
					{//Equipment "id:count:smithing_material"
						vector_drop[0].erase(0);
						iiPtr = new InventoryEquipment(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()),
							(Smithing_Material*)itemLoader->getItem(atoi(vector_drop[2].c_str())));
					}
					else
					{//Normal item "id:count"
						iiPtr = new InventoryItem(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()));
					}
					dropsList->push_back(iiPtr);
				}
			}

			NPC* myChar = new NPC(img_filename, movementspd, attackcd, height, width, cols, colHeight, colWidth, name, description, 9999, false, dropsList);//almost infinite health
			myChar->setTalkText(talk);
			map_npcs[id] = myChar;
		}
		characterstream.close();
	}
	//All Enemies
	characterstream.open(enemyDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description;
		int id, atk, str, def, hp, cols, aggro, wander;
		float movementspd, attackcd, dmg_red, width, height, colHeight, colWidth;
		string dropsData;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd >> height >> width >> cols >> colHeight >> colWidth
				>> description >> atk >> str >> def >> hp >> dmg_red >> aggro >> wander >> dropsData;
			std::replace(name.begin(), name.end(), '_', ' ');
			std::replace(description.begin(), description.end(), '_', ' ');

			//get drops data
			
			vector<InventoryItem*>* dropsList = nullptr;
			
			dropsData.erase(dropsData.begin());
			dropsData.erase(dropsData.end()-1);
			if (dropsData.size() > 0)
			{
				dropsList = new vector<InventoryItem*>();
				InventoryItem* iiPtr;
				vector<string> vector_dropsData = String_Functions::split(dropsData, ',');
				for (int i = 0, l = vector_dropsData.size(); i < l; ++i)
				{
					vector<string> vector_drop = String_Functions::split(vector_dropsData[i], ':');
					if (vector_drop[0].at(0) == 'F')
					{//Food "id:count:food_state"
						vector_drop[0].erase(0);
						iiPtr = new InventoryFood(itemLoader->getItem(atoi(vector_drop[0].c_str())), atoi(vector_drop[1].c_str()),
							atoi(vector_drop[2].c_str()));
					}
					else if (vector_drop[0].at(0) == 'E')
					{//Equipment "id:count:smithing_material"
						vector_drop[0].erase(0);
						iiPtr = new InventoryEquipment(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()), 
							(Smithing_Material*)itemLoader->getItem(atoi(vector_drop[2].c_str())));
					}
					else
					{//Normal item "id:count"
						iiPtr = new InventoryItem(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()));
					}
					dropsList->push_back(iiPtr);
				}
			}

			Enemy* myChar = new Enemy(img_filename, movementspd, attackcd, height, width, cols, colHeight, colWidth, name, description, hp, atk, str, def, dmg_red, aggro, wander, dropsList);
			map_npcs[id] = myChar;
		}
		characterstream.close();
	}
}