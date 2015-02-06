//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef FISHBEHAVIOR_H
#define FISHBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class Player;
class Entity;
class UI;

class FishBehavior :public Behavior
{
private:
	Player* thePlayer;
	Entity* fishes;
	UI* ui;
public:
	virtual ~FishBehavior() {  }
	FishBehavior(Player* p, Entity* f, UI* u)
	{
		thePlayer = p;
		fishes = f;
		ui = u;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif