#ifndef COOKBEHAVIOR_H
#define COOKBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class Player;

using namespace std;

class CookBehavior :public Behavior
{
private:
	Entity* food;
	Player* player;
public:
	CookBehavior(Player* p, Entity* e){
		player = p;
		food = e;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif