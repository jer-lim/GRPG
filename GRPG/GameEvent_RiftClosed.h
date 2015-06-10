#ifndef _GAMEEVENT_RIFTCLOSED_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_RIFTCLOSED_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include <string>

class Rift;

//The player has closed the rift
//On creation for a quest, pass in nullptr to the rift.
class GameEvent_RiftClosed : public GameEvent
{
private:
	Rift* theRift;
public:
	//GameEvent_Damage(){}

	//Generate a new GameEvent with the required comparison operators to be fired off to the GameEvent manager.
	GameEvent_RiftClosed(Rift* r) : GameEvent("", "")
	{
		theRift = r;
	}

	//Generate a new GameEvent to be stored into quest conditions, where string before and after represents text that
	//will be shown in the quest journal before and after the quest is completed.
	GameEvent_RiftClosed(Rift* r, string before, string after) : GameEvent(before, after)
	{
		theRift = r;
	}
	virtual ~GameEvent_RiftClosed()
	{
		theRift = nullptr;
	}
	Rift* getRift() { return theRift; }

	virtual bool is(GameEvent* ge){
		if (getType() == ge->getType())
		{
			return true;
		}
		else return false;
	}
	virtual string getType(){ return "GAMEEVENT_RIFTCLOSED"; }
};

#endif