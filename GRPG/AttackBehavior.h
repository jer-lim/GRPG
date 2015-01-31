#ifndef ATTBEHAVIOR_H
#define ATTBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
using namespace std;

class AttackBehavior :public Behavior
{
private:
	Entity* entity;
	NPC* npc;
public:
	AttackBehavior(Entity* e,NPC* n){ 
		entity = e;
		npc = n;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();//player attacks enemy, and not anything else (is NOT a replacement for entity::attack)
};
#endif