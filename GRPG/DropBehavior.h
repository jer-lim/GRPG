// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef DROPBEHAVIOR_H
#define DROPBEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
using namespace std;

class DropBehavior : public Behavior
{
private:
	Game* gamePtr;
	DrawManager* drawManager;
	Player* player;
	Entity* entity;
public:
	DropBehavior(Game* game, DrawManager* drawMngr, Entity* e, Player* plyr){
		gamePtr = game;
		drawManager = drawMngr;
		entity = e;
		player = plyr;
	}
	bool exists(){ return true; }
	string displayText(){ return "Drop "+entity->getInventoryItem()->getName(); }
	void action();
};
#endif