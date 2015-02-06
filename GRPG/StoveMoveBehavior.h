//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#ifndef _STOVE_MOVE_H
#define _STOVE_MOVE_H

#include "Behavior.h"
#include "entity.h"

class NPC;
using namespace std;
class Player;
class UI;

class StoveMoveBehavior :public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
public:
	StoveMoveBehavior(Player* p, Entity* e, UI* u){
		player = p;
		entity = e;
		ui = u;
	}
	virtual ~StoveMoveBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	//player attacks enemy, and not anything else (is NOT a replacement for entity::attack)
	virtual void action();
};
#endif