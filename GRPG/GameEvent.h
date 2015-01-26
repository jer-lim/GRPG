#ifndef _GAMEEVENT_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_H                 // file is included in more than one place

#include "constants.h"
#include "Entity.h"
#include <string>

class GameEvent
{
private:
	Entity* fromEntity = nullptr, *toEntity = nullptr;
public:
	GameEvent(){}
	~GameEvent(){ 
		fromEntity = nullptr;
		toEntity = nullptr;
	}
	bool is(GameEvent ge){
		return true;
	}
	void setFromEntity(Entity* cha){ fromEntity = cha; }
	Entity* getFromEntity(){ return fromEntity; }
	void settoEntity(Entity* cha){ toEntity = cha; }
	Entity* gettoEntity(){ return toEntity; }
};

#endif