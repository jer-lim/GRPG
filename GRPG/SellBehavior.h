//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _SELLBEHAVIOR_H
#define _SELLBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"

class NPC;
using namespace std;
class Player;
class ItemLoader;
class Grpg;

class SellBehavior : public Behavior
{
private:
	Entity* theItem;
	Player* player;
	Grpg* grpg;

public:
	SellBehavior(Player* p, Entity* i, Grpg* gamePtr){
		player = p;
		theItem = i;
		grpg = gamePtr;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};
#endif