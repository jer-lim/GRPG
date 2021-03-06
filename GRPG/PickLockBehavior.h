#ifndef PICKLOCK_BEHAVIOR_H
#define PICKLOCK_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class QuestData;

class PickLockBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	QuestData* questData;
public:
	PickLockBehavior(Player* p, Entity* e, UI* u, QuestData* qd){
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