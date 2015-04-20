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
	//InventoryItem* reward;//lazy
	int gold;
	Button* ui_element;
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
	Quest(GameEventManager* qcM, string nama, string descript, int gp, Button* b);

	int eventOccured(GameEvent* ge,UI* ui){
		int result;
		for (int i = 0; i < completeConditions.size(); i++)
		{
			//Can only complete a part of the quest that you have started.
			if (completeConditions[i].prereq.size() == 0)
			{
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
		RequiredCondition rc = RequiredCondition();
		rc.completeCondition = qc;
		completeConditions.push_back(rc);
	}

	void addQuestCondition(QuestCondition* qc, vector<QuestCondition*> req)
	{
		RequiredCondition rc = RequiredCondition();
		rc.completeCondition = qc;
		rc.prereq = req;
		completeConditions.push_back(rc); 
	}

	int getgold(){ return gold; }
	void setgold(int n){ gold = n; }
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
	}
};

#endif