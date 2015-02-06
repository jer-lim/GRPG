//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef pkupBEHAVIOR_H
#define pkupBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
using namespace std;

class PickupBehavior :public Behavior
{
private:
	Game* gamePtr;
	DrawManager* drawManager;
	Entity* entity;
	Player* player;
public:
	PickupBehavior(Game* game,DrawManager* drawMngr,Entity* e,Player* plyr){ 
		gamePtr = game;
		drawManager = drawMngr;
		entity = e;
		player = plyr;
	}
	bool exists(){ return true; }
	string displayText(){ return "Pick up "+entity->getInventoryItem()->getName(); }
	void action();
};
#endif