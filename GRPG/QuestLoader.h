//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef QUESTLOADER_H
#define QUESTLOADER_H

#include "constants.h"
#include "Quest.h"
#include "GameEventManager.h"
#include "GameEvent.h"
#include "GameEvent_EntityAction.h"
#include "GameEvent_ItemReceived.h"
#include "GameEvent_ItemUsed.h"
#include "GameEvent_RiftClosed.h"
#include "GameEvent_Damage.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "string_functions.h"
#include "QuestData.h"

class QuestLoader{
private:
	const std::string questLocation = "assets/quests/quests.gdef";
	map<int, Quest*> mapQuests;
	int currentlyShownIndex = -1;
	QuestData* questData;
public:
	QuestLoader() 
	{
		questData = new QuestData();
		questData->init();
	}

	~QuestLoader(){
		for (map<int, Quest*>::iterator it = mapQuests.begin(); it != mapQuests.end(); ++it){
			SAFE_DELETE(it->second);
		}
		mapQuests.clear();
		delete questData;
		questData = nullptr;
	}
	void loadAllQuests(GameEventManager* gem, PersonLoader* personLoader, ItemLoader* itemLoader, Graphics* g,float uiX,float uiY);
	map<int, Quest*>* getMapQuests() {
		return &mapQuests;
	}
	Quest* getQuest(int id)
	{
		return mapQuests[id];
	}
	int getCurrentlyShownIndex(){ return currentlyShownIndex; }
	void setCurrentlyShownIndex(int i){ currentlyShownIndex = i; }

	virtual QuestData* getQuestData()
	{
		return questData;
	}
};
#endif