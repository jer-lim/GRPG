#ifndef _GAMEEVENT_ITEMRECEIVED_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_ITEMRECEIVED_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include "item.h"
#include <string>

//Represents an item that should have been received with a quest
class GameEvent_ItemReceived : public GameEvent
{
private:
	Item* theItem;
public:
	//GameEvent_Damage(){}

	//Generate a new GameEvent with the required comparison operators to be fired off to the GameEvent manager.
	GameEvent_ItemReceived(Item* i) : GameEvent("", "")
	{
		theItem = i;
	}

	//Generate a new GameEvent to be stored into quest conditions, where string before and after represents text that
	//will be shown in the quest journal before and after the quest is completed.
	GameEvent_ItemReceived(Item* i, string before, string after) : GameEvent(before, after)
	{
		theItem = i;
	}
	virtual ~GameEvent_ItemReceived()
	{
		theItem = nullptr;
	}
	Item* getItem() { return theItem; }

	virtual bool is(GameEvent* ge){
		if (getType() == ge->getType())
		{
			return theItem == ((GameEvent_ItemReceived*)ge)->getItem();
		}
		else return false;
	}
	virtual string getType(){ return "GAMEEVENT_ITEMRECEIVED"; }
};

#endif