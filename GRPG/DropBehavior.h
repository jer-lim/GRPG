#ifndef DROPBEHAVIOR_H
#define DROPBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
using namespace std;

class DropBehavior : public Behavior
{
private:
	DrawManager* drawManager;
	Player* player;
	Entity* entity;
public:
	DropBehavior(DrawManager* drawMngr, Entity* e, Player* plyr){
		drawManager = drawMngr;
		entity = e;
		player = plyr;
	}
	bool exists(){ return true; }
	string displayText(){ return "Drop "+entity->getInventoryItem()->getItem()->getName(); }
	void action();
};
#endif