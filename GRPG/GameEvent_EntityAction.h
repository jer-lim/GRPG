#ifndef _GAMEEVENT_ENTITYACTION_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_ENTITYACTION_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include <string>

//Represents an action that was supposed to be done with that entity
//Could be anything, but only works if only one quest is planned for that entity
//For example "Fix this machine in this quest"
class GameEvent_EntityAction : public GameEvent
{
private:
	Person* entity;
public:
	//GameEvent_Damage(){}

	//Generate a new GameEvent with the required comparison operators to be fired off to the GameEvent manager.
	GameEvent_EntityAction(Person* e) : GameEvent("", "")
	{
		entity = e;
	}

	//Generate a new GameEvent to be stored into quest conditions, where string before and after represents text that
	//will be shown in the quest journal before and after the quest is completed.
	GameEvent_EntityAction(Person* e, string before, string after) : GameEvent(before, after)
	{
		entity = e;
	}
	virtual ~GameEvent_EntityAction()
	{
		entity = nullptr;
	}
	Person* getEntity() { return entity; }

	virtual bool is(GameEvent* ge){
		if (GameEvent::is(ge))
		{
			if (getType() == ge->getType())
			{
				//either a kill event or a damage event
				return entity == ((GameEvent_EntityAction*)ge)->getEntity();
			}
			else return false;
		}
		else return false;
	}
	virtual string getType(){ return "GAMEEVENT_ENTITYACTION"; }
};

#endif