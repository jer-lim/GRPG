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
class QuestCondition
{
private:
	GameEvent* successGameEvent;
	int currentCount;
	int countRequirement;
public:
	~QuestCondition(){
		delete successGameEvent;
	}
	QuestCondition(GameEvent* event,int timesRequired){
		currentCount = 0;
		successGameEvent = event;
		countRequirement = timesRequired;
	}
	bool completed(){ return currentCount >= countRequirement; }
	void reset(){
		currentCount = 0;
	}
	int getCurrentCount(){ return currentCount; }
	int getCountRequirement() { return countRequirement; }
	int eventOccured(GameEvent* ge);
};

#endif