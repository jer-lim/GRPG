//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "PersonLoader.h"
#include "Enemy.h"
#include "NPC.h"
#include <algorithm>
#include "InventoryBoost.h"

void PersonLoader::loadAllNPCs(ItemLoader* itemLoader)
{

	ifstream characterstream;
	//Friendly Characters
	characterstream.open(friendlynpcDataLocation);
	if (characterstream.is_open()){
		string name, img_filename, description, talk, dropsData, stealsData;
		char tileID;
		int id, cols, sl;
		float movementspd, attackcd, height, width, colHeight, colWidth;
		while (!characterstream.eof()){
			characterstream >> id >> name >> img_filename >> movementspd >> attackcd >> height >> width >> cols >> colHeight >> colWidth >> description >> talk >> tileID >> dropsData >> stealsData >> sl;
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
						vector_drop[0].erase(0,1);
						iiPtr = new InventoryFood(itemLoader->getItem(atoi(vector_drop[0].c_str())), atoi(vector_drop[1].c_str()),
							static_cast<FOOD_STATE>(atoi(vector_drop[2].c_str())));
					}
					else if (vector_drop[0].at(0) == 'E')
					{//Equipment "id:count:smithing_material"
						vector_drop[0].erase(0,1);
						iiPtr = new InventoryEquipment(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()),
							(Smithing_Material*)itemLoader->getItem(atoi(vector_drop[2].c_str())));
					}
					else if (vector_drop[0].at(0) == 'B')
					{//Boost item "id:count"
						vector_drop[0].erase(0, 1);
						iiPtr = new InventoryBoost(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()));
					}
					else
					{//Normal item "id:count"
						iiPtr = new InventoryItem(itemLoader->getItem(atoi(vector_drop[0].c_str())),
							atoi(vector_drop[1].c_str()));
					}
					dropsList->push_back(iiPtr);
				}
			}

			//Get steal items data
			vector<Item*>* stealList = nullptr;

			stealsData.erase(stealsData.begin());
			stealsData.erase(stealsData.end() - 1);
			if (stealsData.size() > 0)
			{
				stealList = new vector<Item*>();
				Item* iPtr;
				vector<string> vector_stealData = String_Functions::split(stealsData, ',');
				for (int i = 0, l = vector_stealData.size(); i < l; ++i)
				{
					stealList->push_back(itemLoader->getItem(atoi(vector_stealData[i].c_str())));
				}
			}

			NPC* myChar = new NPC(img_filename, movementspd, attackcd, height, width, cols, colHeight, colWidth, name, description, 9999, false, dropsList, stealList, sl);//almost infinite health
			myChar->setTalkText(talk);
			myChar->setTeleportID(tileID);
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
						vector_drop[0].erase(0, 1);
						iiPtr = new InventoryFood(itemLoader->getItem(atoi(vector_drop[0].c_str())), atoi(vector_drop[1].c_str()),
							static_cast<FOOD_STATE>(atoi(vector_drop[2].c_str())));
					}
					else if (vector_drop[0].at(0) == 'E')
					{//Equipment "id:count:smithing_material"
						vector_drop[0].erase(0, 1);
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