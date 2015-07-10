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

string Quest::getQuestString()
{
	stringstream result;
	if (rewardGiven)
	{
		result << "1";
	}
	else
	{
		result << "0";
	}
	result << ":";
	for (vector<RequiredCondition>::iterator i = completeConditions.begin(); i != completeConditions.end(); ++i)
	{
		vector<GameEventStorage>* eventsRequired = i->completeCondition->getConditions();
		for (vector<GameEventStorage>::iterator ii = eventsRequired->begin(); ii != eventsRequired->end(); ++ii)
		{
			//The comma here might imply that when reading later, on the last item, the last comma should be ignored
			//or not read. In actuality this is fine because the String function split will ignore empty strings.
			result << ii->currentCount << ",";
		}
		//Same thing with the colon here, as described in the above comment.
		result << ":";
	}
	return result.str();
}

void Quest::loadQuestString(string s)
{
	vector<string> vector_questData = String_Functions::split(s, ':');
	if (vector_questData[0] == "1")
	{
		rewardGiven = true;
		//Delete rewards, already given
		for (vector<InventoryItem*>::iterator i = itemRewards.begin(); i != itemRewards.end(); i++)
		{
			if ((*i)->getType() == "INVENTORYEQUIPMENT")
			{
				//Smithing material may not be initalized yet
				((InventoryEquipment*)(*i))->setSmithingMaterial(nullptr);
			}
			delete *i;
		}
	}
	else
	{
		rewardGiven = false;
	}

	for (int i = 1, l = vector_questData.size(); i < l; ++i)
	{
		vector<string> vector_questConditionData = String_Functions::split(vector_questData[i], ',');
		
		vector<GameEventStorage>* questConditionConditions = completeConditions[i - 1].completeCondition->getConditions();
		
		for (int ii = 0; ii < questConditionConditions->size(); ii++)
		{
			int newValue = stoi(vector_questConditionData[ii]);
			questConditionConditions->at(ii).currentCount = newValue;
			//Update help text if completed
			if (newValue >= questConditionConditions->at(ii).countRequirement)
			{
				questConditionConditions->at(ii).helpText = questConditionConditions->at(ii).successGameEvent->getAfterText();
			}
		}

		//Now remove all prereqs if this condition is marked as completed
		if (completeConditions[i - 1].completeCondition->completed())
		{
			for (int j = 0; j < completeConditions.size(); j++)
			{
				for (int k = 0; k < completeConditions[j].prereq.size(); k++)
				{
					if (completeConditions[j].prereq[k] == completeConditions[i - 1].completeCondition)
					{
						completeConditions[j].prereq.erase(completeConditions[j].prereq.begin() + k);

					}
				}
			}
		}
	}
}