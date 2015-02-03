#ifndef _QUEST_H                 // Prevent multiple definitions if this 
#define _QUEST_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "QuestCondition.h"
#include "InventoryItem.h"

class Quest
{
private:
	string name;
	string description;
	QuestCondition beginCondition, completeCondition;
	bool completed;
	InventoryItem* reward;
public:
	Quest(){}
};

#endif