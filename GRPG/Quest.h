#ifndef _QUEST_H                 // Prevent multiple definitions if this 
#define _QUEST_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "QuestCondition.h"
#include "InventoryItem.h"
#include "GameEventManager.h"

class Quest
{
private:
	string name;
	string description;
	QuestCondition* completeCondition;//beginCondition, completeCondition;
	//bool completed;
	//InventoryItem* reward;//lazy
	int gold;
public:
	~Quest(){
		delete completeCondition;
		//delete reward;
	}
	Quest(GameEventManager* qcM,string nama, string descript, QuestCondition* completeCond, int gp)//InventoryItem* prize)
	{
		name = nama;
		description = descript;
		completeCondition = completeCond;
		gold = gp;
		qcM->addListener(completeCond);
		//reward = prize;
	}
};

#endif