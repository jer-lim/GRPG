//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _AGGRESSIVEPLUCK_H
#define _AGGRESSIVEPLUCK_H

#include "Behavior.h"
#include "entity.h"
#include "GameEventManager.h"
using namespace std;
class Player;
class NPC;

class AggressivePluckBehavior : public Behavior
{
private:
	Player* player;
	NPC* npc;
	Entity* theChicken;
	UI* ui;
	GameEventManager* gem;
public:
	AggressivePluckBehavior(Player* p, NPC* n, Entity* e, UI* u, GameEventManager* g){
		player = p;
		npc = n;
		theChicken = e;
		ui = u;
		gem = g;
	}
	virtual ~AggressivePluckBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif