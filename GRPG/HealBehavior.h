//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _HEAL_H
#define _HEAL_H

#include "Behavior.h"
#include "entity.h"
using namespace std;
class Player;
class NPC;

class HealBehavior : public Behavior
{
private:
	Player* player;
	NPC* npc;
	Entity* guyDoingHealing;

public:
	HealBehavior(Player* p, NPC* n, Entity* e){
		player = p;
		npc = n;
		guyDoingHealing = e;
	}
	virtual ~HealBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif