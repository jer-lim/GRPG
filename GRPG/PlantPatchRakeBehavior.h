#ifndef PLANTPATCHRAKE_BEHAVIOR_H
#define PLANTPATCHRAKE_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class QuestData;

class PlantPatchRakeBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	QuestData* questData;
public:
	PlantPatchRakeBehavior(Player* p, Entity* e, UI* u, QuestData* qd){
		player = p;
		entity = e;
		ui = u;
		questData = qd;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif