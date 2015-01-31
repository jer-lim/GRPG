#ifndef pkupBEHAVIOR_H
#define pkupBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
using namespace std;

class PickupBehavior :public Behavior
{
private:
	Entity* entity;
public:
	PickupBehavior(Entity* e){ entity = e; }
	bool exists(){ return true; }
	string displayText(){ return "Pick up "+entity->getInventoryItem()->getItem()->getName(); }
	void action(){}
};
#endif