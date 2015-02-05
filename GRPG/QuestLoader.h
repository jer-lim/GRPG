#ifndef QUESTLOADER_H
#define QUESTLOADER_H

#include "constants.h"
#include "Quest.h"
#include "GameEventManager.h"
#include "GameEvent_Damage.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "string_functions.h"

class QuestLoader{
private:
	const std::string questLocation = "assets/quests/quests.gdef";
	unordered_map<int, Quest*> mapQuests;
public:
	QuestLoader(){}
	~QuestLoader(){
		for (unordered_map<int, Quest*>::iterator it = mapQuests.begin(); it != mapQuests.end(); ++it){
			SAFE_DELETE(it->second);
		}
		mapQuests.clear();
	}
	void loadAllQuests(GameEventManager* gem,PersonLoader* personLoader);
	unordered_map<int, Quest*>* getMapQuests() {
		return &mapQuests;
	}
	Quest* getQuest(int id)
	{
		return mapQuests[id];
	}
};
#endif