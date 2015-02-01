#ifndef MINEBEHAVIOR_H
#define MINEBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class Player;
class Entity;
class UI;

class MineBehavior :public Behavior
{
private:
	Player* thePlayer;
	Entity* mine;
	UI* ui;
public:
	MineBehavior(Player* p, Entity* m, UI* u)
	{
		thePlayer = p;
		mine = m;
		ui = u;
	}
	virtual ~MineBehavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return "Mine"; }
	virtual void action(){}
};
#endif