#include "Quest.h"
#include "GameEventManager.h"

Quest::Quest(GameEventManager* qcM, string nama, string descript, QuestCondition* completeCond, int gp, Button* b)
{
	name = nama;
	description = descript;
	completeCondition = completeCond;
	gold = gp;
	ui_element = b;
	qcM->addListener(this);
	//reward = prize;
}