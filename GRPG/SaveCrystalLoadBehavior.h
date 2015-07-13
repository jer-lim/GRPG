#ifndef SAVECRYSTAL_LOAD_BEHAVIOR_H
#define SAVECRYSTAL_LOAD_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class Grpg;

class SaveCrystalLoadBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	Grpg* grpg;
public:
	SaveCrystalLoadBehavior(Player* p, Entity* e, UI* u){
		player = p;
		entity = e;
		ui = u;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif