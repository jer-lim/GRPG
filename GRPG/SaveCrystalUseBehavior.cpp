#include "SaveCrystalUseBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "SaveCrystal.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include <string>
#include "grpg.h"

#include <cereal\types\map.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\types\complex.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal\cereal.hpp>

#include <iostream>
#include <fstream>

string SaveCrystalUseBehavior::displayText(){
	return "Use Save Crystal";
}

void SaveCrystalUseBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		/*map<string, map<string, int>> saveData;
		ui->addChatText("Save data saved.");
		//ui->drawWindow("Save Crystal");
		//ui->addTalkText(new ChatInformation("This "))
		saveData["questData"] = grpg->getQuestLoader()->getQuestData()->getAllValues();
		saveData["skillsData"] = player->getSkillsToSave();
		map<string, int> otherData;
		otherData[player->getInventory()->getInventoryString()] = 0;
		saveData["inventory"] = otherData;
		map<string, int> questData;
		map<int, Quest*>* questList = grpg->getQuestLoader()->getMapQuests();
		for (map<int, Quest*>::iterator i = questList->begin(); i != questList->end(); ++i)
		{
			questData[i->second->getQuestString()] = i->first;
		}
		saveData["quests"] = questData;

		ofstream myfile;
		myfile.open("savefile.txt");
		cereal::JSONOutputArchive output(myfile);
		output(cereal::make_nvp("savedata", saveData));*/

		grpg->getDrawManager()->removeAllDroppedItems();
		
		ui->addChatText("Save data loaded.");
		map<string, map<string, int>> loadData;
		ifstream loadFile("savefile.txt");
		if (loadFile.is_open())
		{
			cereal::JSONInputArchive i_archive(loadFile);
			i_archive(loadData);
		}
		player->loadSkills(loadData["skillsData"]);
		grpg->getQuestLoader()->getQuestData()->loadQuestData(loadData["questData"]);
		vector<string> keys;
		for (map<string, int>::iterator i = loadData["inventory"].begin(); i != loadData["inventory"].end(); i++)
		{
			keys.push_back(i->first);
		}
		player->getInventory()->loadInventoryString(keys[0], grpg->getItemLoader(), grpg);
		//Delete and reload all quests, they may have progressed passed their save state
		map<int, Quest*>* mapQuests = grpg->getQuestLoader()->getMapQuests();
		for (map<int, Quest*>::iterator i = mapQuests->begin(); i != mapQuests->end(); ++i)
		{
			//Remove them
			grpg->getGameEventManager()->removeListener(i->second);
			SAFE_DELETE(i->second);
		}
		mapQuests->clear();

		grpg->getQuestLoader()->loadAllQuests(grpg->getGameEventManager(), grpg->getPersonLoader(), grpg->getItemLoader(), grpg->getGraphics(), ui->getTopLeftX(), ui->getTopLeftY());
		mapQuests = grpg->getQuestLoader()->getMapQuests();
		for (map<string, int>::iterator i = loadData["quests"].begin(); i != loadData["quests"].end(); i++)
		{
			mapQuests->at(i->second)->loadQuestString(i->first);
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}