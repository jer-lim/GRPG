//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _EASTERINTERACT_H
#define _EASTERINTERACT_H

#include "Behavior.h"
#include "entity.h"
#include "GameEventManager.h"
using namespace std;
class Player;
class NPC;
class Grpg;

class EasterInteractBehavior : public Behavior
{
private:
	Player* player;
	NPC* npc;
	Entity* theBird;
	UI* ui;
	GameEventManager* gem;
	QuestData* questData;
	Grpg* grpg;
public:
	EasterInteractBehavior(Player* p, NPC* n, Entity* e, UI* u, GameEventManager* g, QuestData* qd, Grpg* theGame){
		player = p;
		npc = n;
		theBird = e;
		ui = u;
		gem = g;
		questData = qd;
		grpg = theGame;
	}
	virtual ~EasterInteractBehavior() {}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};

#endif