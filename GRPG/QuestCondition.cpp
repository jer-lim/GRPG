#include "QuestCondition.h"

//return 0 if nothing happened, 1 if success, 2 if condition completed
int QuestCondition::eventOccured(GameEvent ge)
{
	///TODO: INCLUDE GAME EVENT COMPARISON
	if (successGameEvent.is(ge))
	{
		++currentCount;
		if (currentCount >= countRequirement)
			return 2;
		else return 1;
	}
	return 0;
}