//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "QuestCondition.h"

//return 0 if nothing happened, 1 if success, 2 if condition completed
int QuestCondition::eventOccured(GameEvent* ge)
{
	int result = NO_CHANGE;
	for (int i = 0; i < conditions.size(); i++)
	{
		///TODO: INCLUDE GAME EVENT COMPARISON
		if (conditions[i].prereq == nullptr)
		{
			//Going to assume that once a match is found that means that there will be definitely no other
			//matches in the later game events. I think that's a resonable assumption
			//(Why would a quest require multiple of the same gameevent at different stages?)
			if (conditions[i].successGameEvent->is(ge))
			{
				result = COMPLETED;
				conditions[i].currentCount++;
				if (conditions[i].currentCount >= conditions[i].countRequirement)
				{
					//Also update all pre-req
					for (int j = 0; j < conditions.size(); j++)
					{
						if (conditions[j].prereq == conditions[i].successGameEvent)
						{
							conditions[j].prereq = nullptr;
						}
					}
					conditions[i].helpText = conditions[i].successGameEvent->getAfterText();
					conditions[i].successGameEvent->performChange(questData);
				}
				else
				{
					result = QUEST_SUCCESS;
				}
			}
		}
	}
	if (result != COMPLETED)
	{
		return NO_CHANGE;
	}
	else
	{
		for (int i = 0; i < conditions.size(); i++)
		{
			if (conditions[i].currentCount < conditions[i].countRequirement)
			{
				return QUEST_SUCCESS;
			}
		}
		return COMPLETED;
	}
}