#ifndef _QUESTCONDITION_H                 // Prevent multiple definitions if this 
#define _QUESTCONDITION_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include <string>

class QuestCondition
{
private:
	GameEvent successGameEvent;
	int currentCount;
	int countRequirement;
public:
	QuestCondition(){}
	bool completed(){return currentCount >= countRequirement;}
	int eventOccured(GameEvent);
};

#endif