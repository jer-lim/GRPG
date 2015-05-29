//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _QUESTCONDITION_H                 // Prevent multiple definitions if this 
#define _QUESTCONDITION_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include "UI.h"
#include <string>
enum QUEST_UPDATE { NO_CHANGE, QUEST_SUCCESS, COMPLETED };

struct GameEventStorage
{
	GameEvent* successGameEvent;
	int countRequirement;
	int currentCount;
	string helpText;
	GameEvent* prereq;
};

class QuestCondition
{
private:
	vector<GameEventStorage> conditions;
	//This questData is not passed in during the constructor, but is instead set by the Quest class
	//When the condition is attached to it.
	QuestData* questData;
	//Marks if the quest condition is required to be completed for the quest to be fully completed
	//or if it's optional/
	bool required;
public:
	~QuestCondition(){
		for (int i = 0; i < conditions.size(); i++)
		{
			delete conditions[i].successGameEvent;
		}
	}
	QuestCondition() {
		required = true;
	}

	QuestCondition(bool r) {
		required = r;
	}

	void addGameEventRequirement(GameEvent* event, int timesRequired, GameEvent* prereq)
	{
		GameEventStorage ges = GameEventStorage();
		ges.successGameEvent = event;
		ges.prereq = prereq;
		ges.countRequirement = timesRequired;
		ges.currentCount = 0;
		ges.helpText = event->getBeforeText();
		conditions.push_back(ges);
	}

	bool completed()
	{
		for (int i = 0; i < conditions.size(); i++)
		{
			if (conditions[i].currentCount < conditions[i].countRequirement)
			{
				return false;
			}
		}
		return true;
	}

	void reset()
	{
		for (int i = 0; i < conditions.size(); i++)
		{
			conditions[i].currentCount = 0;
		}
	}

	virtual string getFullHelpText()
	{
		string s;
		for (int i = 0; i < conditions.size(); i++)
		{
			if (conditions[i].prereq == nullptr)
			{
				s += conditions[i].helpText;
			}
		}
		return s;
	}

	virtual void setQuestData(QuestData* qd) { questData = qd; }
	virtual QuestData* getQuestData() { return questData; }
	virtual void setRequired(bool r) { required = r; }
	virtual bool getRequired() { return required; }

	//int getCurrentCount(){ return currentCount; }
	//int getCountRequirement() { return countRequirement; }
	int eventOccured(GameEvent* ge);
};

#endif