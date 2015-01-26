#ifndef _GAMEEVENT_DAMAGE_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_DAMAGE_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include <string>

class GameEvent_Damage : public GameEvent
{
private:
	int damageInflicted;
public:
	GameEvent_Damage(){}
	GameEvent_Damage(int i){ damageInflicted = i; }
	bool isKill(){
		
	}
};

#endif