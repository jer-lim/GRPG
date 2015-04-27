//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _QUEST_H                 // Prevent multiple definitions if this 
#define _QUEST_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "QuestCondition.h"
#include "InventoryItem.h"
//#include "GameEventManager.h"
#include "Button.h"
#include "QuestData.h"

class GameEventManager;

namespace QuestNS{
	const float INITIAL_DISPLACEMENT = 40;
	const float SIDE_DISPLACEMENT = 10;
	const float MARGIN = 10;
	const float WIDTH = 150;
	const float HEIGHT = 20;
	const DWORD BACK_COLOR = graphicsNS::BLUE;

}

struct RequiredCondition
{
	QuestCondition* completeCondition;
	vector<QuestCondition*> prereq;
};

class Quest
{
private:
	string name;
	string description;
	vector<RequiredCondition> completeConditions;//beginCondition, completeCondition;
	//bool completed;
	Button* ui_element;
	QuestData* questData;
	map<int, int> skillReward;
	vector<InventoryItem*> itemRewards;
	bool rewardGiven;
public:
	~Quest(){
		for (int i = 0; i < completeConditions.size(); i++)
		{
			SAFE_DELETE(completeConditions[i].completeCondition);
		}
		completeConditions.clear();
		SAFE_DELETE(ui_element);	
		//delete reward;//changed to int lol lazy boy ah matthew
	}
	Quest(GameEventManager* qcM, QuestData* qd, string nama, string descript, int gp, Button* b);

	int eventOccured(GameEvent* ge,UI* ui){
		int result = NO_CHANGE;
		for (int i = 0; i < completeConditions.size(); i++)
		{
			//Can only complete a part of the quest that you have started.
			if (completeConditions[i].prereq.size() == 0)
			{
				if (completeConditions[i].completeCondition->completed())
				{
					continue;
				}
				result = completeConditions[i].completeCondition->eventOccured(ge);
				switch (result)
				{
				case QUEST_SUCCESS:
					//ui->addChatText("You have made progress in quest " + name + "!");
					break;
				case COMPLETED:
					//ui->addChatText("You have completed quest " + name + "!\nProceed to the Quest Board for your reward!");
					//Update the quest prereq for other parts as well
					for (int j = 0; j < completeConditions.size(); j++)
					{
						for (int k = 0; k < completeConditions[j].prereq.size(); k++)
						{
							if (completeConditions[j].prereq[k] == completeConditions[i].completeCondition)
							{
								completeConditions[j].prereq.erase(completeConditions[j].prereq.begin() + k);

							}
						}
					}
					break;
				}
				if (result != NO_CHANGE)
				{
					break;
				}
			}
		}
		if (result != COMPLETED)
		{
			return result;
		}
		else
		{
			for (int i = 0; i < completeConditions.size(); i++)
			{
				if (!completeConditions[i].completeCondition->completed())
				{
					return QUEST_SUCCESS;
				}
			}
			return COMPLETED;
		}
	}

	bool completed() {
		for (int i = 0; i < completeConditions.size(); i++)
		{
			if (!completeConditions[i].completeCondition->completed())
			{
				return false;
			}
		}
		return true;
	}

	void reset()
	{
		for (int i = 0; i < completeConditions.size(); i++)
		{
			completeConditions[i].completeCondition->reset();
		}
	}

	string getname(){ return name; }
	void setname(string n){ name = n; }
	string getdescript(){ return description; }
	void setdescript(string n){ description = n; }
	vector<RequiredCondition> getQuestConditions() { return completeConditions; }

	//Adds a new condition to the quest without a prerequsite.
	void addQuestCondition(QuestCondition* qc)
	{
		qc->setQuestData(questData);
		RequiredCondition rc = RequiredCondition();
		rc.completeCondition = qc;
		completeConditions.push_back(rc);
	}

	//Adds a new condition to the quest with a vector of prerequsites
	void addQuestCondition(QuestCondition* qc, vector<QuestCondition*> req)
	{
		qc->setQuestData(questData);
		RequiredCondition rc = RequiredCondition();
		rc.completeCondition = qc;
		rc.prereq = req;
		completeConditions.push_back(rc); 
	}

	//Adds a new condition to the quest with only 1 prerequsite.
	void addQuestCondition(QuestCondition* qc, QuestCondition* req)
	{
		qc->setQuestData(questData);
		RequiredCondition rc = RequiredCondition();
		rc.completeCondition = qc;
		rc.prereq.push_back(req);
		completeConditions.push_back(rc);
	}

	//Add a new skill reward to this quest at the very end
	void addSkillReward(int theSkillId, int amount)
	{
		skillReward[theSkillId] = amount;
	}

	//Add a new item as a reward to this quest
	void addItemReward(InventoryItem* ii)
	{
		itemRewards.push_back(ii);
	}

	bool getRewardGiven() { return rewardGiven; }
	Button* getUIElement(){ return ui_element; }
	void setUIElement(Button* b){ ui_element = b; }

	void printHelpTextToUI(UI* ui)
	{
		for (int i = 0; i < completeConditions.size(); i++)
		{
			if (completeConditions[i].prereq.size() == 0)
			{
				ui->addTalkText(new ChatInformation(completeConditions[i].completeCondition->getFullHelpText(), chatNS::MIDDLE));
			}
		}
		if (completed())
		{
			ui->addTalkText(new ChatInformation("Hooray, Quest complete!", chatNS::MIDDLE));
		}
	}

	void gainRewards(UI* ui, Player* p)
	{
		rewardGiven = true;
		for (map<int, int>::iterator i = skillReward.begin(); i != skillReward.end(); i++)
		{
			p->getSkills()->at(i->first).gainXP(i->second);
		}
	}
};

#endif