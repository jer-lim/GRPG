//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "Quest.h"
#include "GameEventManager.h"
#include "grpg.h"
#include "PickupBehavior.h"

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
	for (vector<ConditionalSkillReward>::iterator i = skillRewards.begin(); i != skillRewards.end(); i++)
	{
		if (i->name == "" || questData->getValue(i->name) == i->requiredValue)
		{
			p->getSkills()->at(i->skillToIncrease).gainXP(i->XPtoGain);
		}
	}
	for (map<string, int>::iterator i = questDataChanges.begin(); i != questDataChanges.end(); i++)
	{
		questData->setValue(i->first, i->second);
	}
	for (vector<InventoryItem*>::iterator i = itemRewards.begin(); i != itemRewards.end(); i++)
	{
		InventoryItem* theItem = *i;
		Entity* e = new Entity();
		e->initialize(grpg, theItem, false);
		INVENTORY_CHANGE result = p->getInventory()->addEntityInventoryItem(e, grpg, true);
		if (result == MERGED)
		{
			itemsAllocatedForDeletion.push_back(e);
		}
		else if (result == PARTIAL_MERGE || result == FAILED)
		{
			//Drop the remainder on the ground
			e->setX(p->getX());
			e->setY(p->getY());
			e->setPickupBehavior(new PickupBehavior(grpg, grpg->getDrawManager(), e, grpg->getPlayer()));//change behaviors
			e->setDropBehavior(nullptr);
			e->setupVectorActiveBehaviors();
			e->setAnchored(false);
			grpg->getDrawManager()->addObject(e);
		}
	}
}