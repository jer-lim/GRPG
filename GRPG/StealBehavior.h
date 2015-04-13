//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef STEALBEHAVIOR_H
#define STEALBEHAVIOR_H

#include "Behavior.h"
using namespace std;
class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;
class Player;
class Entity;

class StealBehavior : public Behavior
{
private:
	NPC* ii;
	UI* ui;
	Player* thePlayer;
	Entity* entity; //The entity to steal from
public:
	virtual ~StealBehavior() {}
	StealBehavior(NPC* i, UI* u, Player* p, Entity* e){
		ii = i;
		ui = u;
		thePlayer = p;
		entity = e;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif