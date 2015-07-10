#ifndef SAVECRYSTALUSE_BEHAVIOR_H
#define SAVECRYSTALUSE_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;

class SaveCrystalUseBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
public:
	SaveCrystalUseBehavior(Player* p, Entity* e, UI* u){
		player = p;
		entity = e;
		ui = u;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif