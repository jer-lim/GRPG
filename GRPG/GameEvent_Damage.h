// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _GAMEEVENT_DAMAGE_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_DAMAGE_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include <string>

class GameEvent_Damage : public GameEvent
{
private:
	int damageInflicted;
	bool isKill;
	Person* from = nullptr, *to = nullptr;
public:
	//GameEvent_Damage(){}
	//Generate a new GameEvent with the required comparison operators to be fired off to the GameEvent manager.
	GameEvent_Damage(Person* f, Person* t, int i, bool kill) : GameEvent("", "")
	{
		damageInflicted = i;
		isKill = kill;
		from = f;
		to = t;
	}

	//Generate a new GameEvent to be stored into quest conditions, where string before and after represents text that
	//will be shown in the quest journal before and after the quest is completed.
	GameEvent_Damage(Person* f, Person* t, int i, bool kill, string before, string after) : GameEvent(before, after)
	{
		damageInflicted = i;
		isKill = kill;
		from = f;
		to = t;
	}
	virtual ~GameEvent_Damage()
	{
		from = nullptr;
		to = nullptr;
	}
	int getDamageInflicted(){ return damageInflicted; }
	bool getIsKill(){ return isKill; }
	Person* getFrom() { return from; }
	Person* getTo() { return to; }

	virtual bool is(GameEvent* ge){
		if (getType() == ge->getType())
		{
			if (((GameEvent_Damage*)ge)->getFrom() == from && ((GameEvent_Damage*)ge)->getTo() == to)
			{
				//either a kill event or a damage event
				return (isKill == true && isKill == ((GameEvent_Damage*)ge)->getIsKill()) || damageInflicted > 0 && ((GameEvent_Damage*)ge)->getDamageInflicted() > 0;
			}
			else return false;
		}
		else return false;
	}
	virtual string getType(){ return "GAMEEVENT_DAMAGE"; }
};

#endif