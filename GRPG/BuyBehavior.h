#ifndef _BUYBEHAVIOR_H
#define _BUYBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"

class NPC;
using namespace std;
class Player;

class BuyBehavior : public Behavior
{
private:
	Entity* theItem;
	Player* player;
public:
	BuyBehavior(Player* p, Entity* i){
		player = p;
		theItem = i;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};
#endif