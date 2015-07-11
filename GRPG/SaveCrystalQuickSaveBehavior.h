#ifndef SAVECRYSTAL_QUICKSAVE_BEHAVIOR_H
#define SAVECRYSTAL_QUICKSAVE_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class Grpg;

class SaveCrystalQuickSaveBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	Grpg* grpg;
public:
	SaveCrystalQuickSaveBehavior(Player* p, Entity* e, UI* u){
		player = p;
		entity = e;
		ui = u;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif