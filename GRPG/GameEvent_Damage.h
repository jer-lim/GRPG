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
public:
	//GameEvent_Damage(){}
	GameEvent_Damage(Person* from, Person* to, int i, bool kill)
		: GameEvent(from, to)
	{ 
		damageInflicted = i;
		isKill = kill;
	}
	int getDamageInflicted(){ return damageInflicted; }
	bool getIsKill(){ return isKill; }
	virtual bool is(GameEvent* ge){
		if (GameEvent::is(ge))
		{
			if (getType() == ge->getType())
			{
				//either a kill event or a damage event
				return isKill == isKill || damageInflicted > 0 && ((GameEvent_Damage*)ge)->getDamageInflicted() > 0;
			}
			else return false;
		}
		else return false;
	}
	virtual string getType(){ return "GAMEEVENT_DAMAGE"; }
};

#endif