#ifndef MINEBEHAVIOR_H
#define MINEBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class Player;
class Entity;

class MineBehavior :public Behavior
{
private:
	Player* thePlayer;
	Entity* mine;
public:
	MineBehavior(Player* p, Entity* m)
	{
		thePlayer = p;
		mine = m;
	}
	virtual ~MineBehavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return "Mine"; }
	virtual void action(){}
};
#endif