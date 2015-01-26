#ifndef _GAMEEVENT_LEVELUP_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_LEVELUP_H                 // file is included in more than one place

#include "constants.h"
#include "GameEvent.h"
#include "playerSkill.h"
#include <string>

class GameEvent_LevelUp : public GameEvent
{
private:
	long previousExp;
	PlayerSkill skillLevelledUp;
public:
	GameEvent_LevelUp(){}
};

#endif