//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "QuestCondition.h"

//return 0 if nothing happened, 1 if success, 2 if condition completed
int QuestCondition::eventOccured(GameEvent* ge)
{
	///TODO: INCLUDE GAME EVENT COMPARISON
	if (successGameEvent->is(ge))
	{
		++currentCount;
		if (currentCount >= countRequirement)
		{
			return COMPLETED;
		}
		else
		{
			return QUEST_SUCCESS;
		}
	}
	return NO_CHANGE;
}