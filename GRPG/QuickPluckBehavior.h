//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _QUICKPLUCK_H
#define _QUICKPLUCK_H

#include "Behavior.h"
#include "entity.h"
#include "GameEventManager.h"
using namespace std;
class Player;
class NPC;

class QuickPluckBehavior : public Behavior
{
private:
	Player* player;
	NPC* npc;
	Entity* theChicken;
	UI* ui;
	GameEventManager* gem;
public:
	QuickPluckBehavior(Player* p, NPC* n, Entity* e, UI* u, GameEventManager* g){
		player = p;
		npc = n;
		theChicken = e;
		ui = u;
		gem = g;
	}
	virtual ~QuickPluckBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif