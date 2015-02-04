#ifndef _GAMEEVENT_H                 // Prevent multiple definitions if this 
#define _GAMEEVENT_H                 // file is included in more than one place

#include "constants.h"
#include "Entity.h"
#include <string>

class GameEvent
{
protected:
	Person* fromPerson = nullptr, *toPerson = nullptr;
public:
	//GameEvent(){}
	GameEvent(Person* from, Person* to)
	{
		fromPerson = from;
		toPerson = to;
	}
	~GameEvent(){ 
		//should be deleted elsewhere
		fromPerson = nullptr;
		toPerson = nullptr;
	}
	bool personCheck(GameEvent* ge)
	{
		if (fromPerson != nullptr)
		{
			if (ge->getFromPerson() != nullptr)
			{
				if (fromPerson != ge->getFromPerson())
					return false;
			}
			else return false;
		}
		if (toPerson != nullptr)
		{
			if (ge->gettoPerson() != nullptr)
			{
				if (toPerson != ge->gettoPerson())
					return false;
			}
			else return false;
		}

		return true;
	}
	virtual bool is(GameEvent* ge){
		if (getType() == ge->getType())
		{
			return personCheck(ge);
		}
		else return false;
	}
	void setFromPerson(Person* cha){ fromPerson = cha; }
	Person* getFromPerson(){ return fromPerson; }
	void settoPerson(Person* cha){ toPerson = cha; }
	Person* gettoPerson(){ return toPerson; }
	virtual string getType(){ return "GAMEEVENT"; }
};

#endif