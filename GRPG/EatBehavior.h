#ifndef EATBEHAVIOR_H
#define EATBEHAVIOR_H

// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "Behavior.h"
#include "entity.h"
class Player;

using namespace std;

class EatBehavior : public Behavior
{
private:
	Game* gamePtr;
	Entity* food;
	Player* player;
public:
	EatBehavior(Game* game, Entity* e, Player* p){
		gamePtr = game;
		player = p;
		food = e; 
	}
	bool exists(){ return true; }
	string displayText();
	void action();
};
#endif