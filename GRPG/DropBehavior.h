#ifndef DROPBEHAVIOR_H
#define DROPBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
using namespace std;

class DropBehavior : public Behavior
{
private:
	Entity* entity;
public:
	//DropBehavior(){}
	DropBehavior(Entity* e){ entity = e; }
	bool exists(){ return true; }
	string displayText(){ return "Drop "+entity->getInventoryItem()->getItem()->getName(); }
	void action(){}
};
#endif