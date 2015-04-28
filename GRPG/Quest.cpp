//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "Quest.h"
#include "GameEventManager.h"
#include "grpg.h"

Quest::Quest(GameEventManager* qcM, QuestData* qd, string nama, string descript, Button* b)
{
	name = nama;
	description = descript;
	ui_element = b;
	qcM->addListener(this);
	questData = qd;
	//reward = prize;
}

void Quest::gainRewards(UI* ui, Player* p, Grpg* grpg)
{
	rewardGiven = true;
	for (map<int, int>::iterator i = skillReward.begin(); i != skillReward.end(); i++)
	{
		p->getSkills()->at(i->first).gainXP(i->second);
	}
	for (vector<InventoryItem*>::iterator i = itemRewards.begin(); i != itemRewards.end(); i++)
	{
		InventoryItem* theItem = *i;
		Entity* e = new Entity();
		e->initialize(grpg, theItem, false);
		p->getInventory()->addEntityInventoryItem(e, grpg);
	}
}