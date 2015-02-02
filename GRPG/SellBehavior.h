#ifndef _SELLBEHAVIOR_H
#define _SELLBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"

class NPC;
using namespace std;
class Player;

class SellBehavior : public Behavior
{
private:
	Entity* theItem;
	Player* player;
public:
	SellBehavior(Player* p, Entity* i){
		player = p;
		theItem = i;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};
#endif