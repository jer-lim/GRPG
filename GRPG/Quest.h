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
class Grpg;

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

struct ConditionalSkillReward
{
	//The name in quest data that is required to have so as to grant the reward.
	//If empty string, no such condition and can be ignored.
	string name;
	int requiredValue;
	int skillToIncrease;
	int XPtoGain;
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
	vector<ConditionalSkillReward> skillRewards;
	vector<InventoryItem*> itemRewards;
	//If an item reward is awarded to the player and merged into a stack (for example, an award of coins
	//when the player already has coins), by default Inventory will attempt to delete the object as it is merged
	//We can't do that this time, however, as otherwise we cannot display the quest reward screen
	//with all the items that the quest rewards. To solve this problem, we prevent deletion, and then allocate the item
	//for deletion later, such as when the game is closed (i.e. in destructor)
	//HOWEVER, for some reason the items are already deleted by that time...and attempting to delete them crashes
	//the game. Hmm...
	vector<Entity*> itemsAllocatedForDeletion;
	//Miscallenous rewards that do not belong in either category, but are regardless rewards for the quest
	//These rewards only contain text that should be displayed to the user, the actual reward has to be coded
	//inside the program at the relevant locations.
	vector<string> miscRewards;
	bool rewardGiven;
	map<string, int> questDataChanges;
public:
	~Quest(){
		for (int i = 0; i < completeConditions.size(); i++)
		{
			SAFE_DELETE(completeConditions[i].completeCondition);
		}
		completeConditions.clear();
		//Destroy the reward item if it's not been given to the player
		if (!rewardGiven)
		{
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
			for (vector<Entity*>::iterator i = itemsAllocatedForDeletion.begin(); i != itemsAllocatedForDeletion.end(); i++)
			{
				//I don't get why i don't need to do this. I thought it would leak. It doesn't????
				//SAFE_DELETE(*i);
			}
		}
		SAFE_DELETE(ui_element);	
		//delete reward;//changed to int lol lazy boy ah matthew
	}
	Quest(GameEventManager* qcM, QuestData* qd, string nama, string descript, Button* b);

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
			//IF it wasn't completed and was a required condition
			if (!completeConditions[i].completeCondition->completed() && completeConditions[i].completeCondition->getRequired())
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
	//the player is guaranteed to gain this xp, with no requirements.
	void addSkillReward(int theSkillId, int amount)
	{
		ConditionalSkillReward csr;
		csr.skillToIncrease = theSkillId;
		csr.XPtoGain = amount;
		skillRewards.push_back(csr);
	}

	//Add a new skill reward to this quest at the very end
	//The player will only gain this reward if QuestData's <stringToCheck> is equal to <value> at the end of the quest.
	//Effectively a conditional skill reward that can be configured to only be given if certain conditions are met,
	//checked by questData's condition.
	void addSkillReward(string stringToCheck, int value, int theSkillId, int amount)
	{
		ConditionalSkillReward csr;
		csr.name = stringToCheck;
		csr.requiredValue = value;
		csr.skillToIncrease = theSkillId;
		csr.XPtoGain = amount;
		skillRewards.push_back(csr);
	}

	vector<ConditionalSkillReward>* getSkillsRewards() { return &skillRewards; }

	//Add a new item as a reward to this quest
	void addItemReward(InventoryItem* ii)
	{
		itemRewards.push_back(ii);
	}

	vector<InventoryItem*> getItemRewards() { return itemRewards; }

	void addMiscReward(string s)
	{
		miscRewards.push_back(s);
	}

	//Adds a new quest data change to be performed to the QuestData object once this quest has been successfully marked as completed.
	void addQuestDataChange(string name, int newValue)
	{
		questDataChanges[name] = newValue;
	}
	
	vector<string> getMiscRewards() { return miscRewards; }

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

	void gainRewards(UI* ui, Player* p, Grpg* grpg);

	//Saves the quest data, that can potentially change, into a string so that it can be
	//saved and loaded at any point of time
	//The saved string follows the following format:
	// {rewardGiven}:{conditionCount},{conditionCount},...,:{nextConditionCount},...,:{nextNextConditionCount},:...
	string getQuestString();

	//Loads the quest data as per the string sent in
	//string should be generated from getQuestString()
	void loadQuestString(string s);
};

#endif