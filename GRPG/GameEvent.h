// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _GAMEEVENT_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_H                 // file is included in more than one place

#include "constants.h"
#include "Entity.h"
#include <string>

class GameEvent
{
protected:
	string beforeCompleteText;
	string afterCompleteText;
public:
	GameEvent(string before, string after){
		beforeCompleteText = before;
		afterCompleteText = after;
	}
	virtual ~GameEvent(){ 
		//should be deleted elsewhere
	}

	virtual bool is(GameEvent* ge) = 0;
	virtual string getType(){ return "GAMEEVENT"; }

	virtual string getBeforeText() { return beforeCompleteText; }
	virtual string getAfterText() { return afterCompleteText; }
};

#endif