#ifndef _GAMEEVENT_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_H                 // file is included in more than one place

#include "constants.h"
#include "Character.h"
#include <string>

class GameEvent
{
private:
	Character fromCharacter, toCharacter;
public:
	GameEvent(){}
	bool is(GameEvent ge){
		return true;
	}
};

#endif