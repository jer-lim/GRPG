//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _BUYBEHAVIOR_H
#define _BUYBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"

class NPC;
using namespace std;
class Player;
class Grpg;

class BuyBehavior : public Behavior
{
private:
	Entity* theItem;
	Player* player;
	Entity* coin;
	Grpg* grpg;

public:
	BuyBehavior(Player* p, Entity* i, Entity* c, Grpg* gamePtr){
		player = p;
		theItem = i;
		coin = c;
		grpg = gamePtr;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};
#endif