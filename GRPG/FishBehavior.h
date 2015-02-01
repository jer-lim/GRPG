#ifndef FISHBEHAVIOR_H
#define FISHBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class Player;
class Entity;

class FishBehavior :public Behavior
{
private:
	Player* thePlayer;
	Entity* fishes;
public:
	virtual ~FishBehavior() {  }
	FishBehavior(Player* p, Entity* f)
	{
		thePlayer = p;
		fishes = f;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif