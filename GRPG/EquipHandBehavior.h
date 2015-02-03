#ifndef EQUIPHANDBEHAVIOR_H
#define EQUIPHANDBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
using namespace std;

class EquipHandBehavior :public Behavior
{
private:
	Game* gamePtr;
	Entity* entity;
	Player* player;
public:
	EquipHandBehavior(Game* game, Entity* e, Player* plyr){
		gamePtr = game;
		entity = e;
		player = plyr;
	}
	bool exists(){ return true; }
	string displayText(){ return "Pick up " + entity->getInventoryItem()->getItem()->getName(); }
	void action();
};
#endif