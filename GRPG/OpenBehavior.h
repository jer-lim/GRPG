#ifndef RICHDORMINE_BEHAVIOR_H
#define RICHDORMINE_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class QuestData;

class OpenBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	QuestData* qd;
public:
	OpenBehavior(Player* p, Entity* e, UI* u, QuestData* q){
		player = p;
		entity = e;
		ui = u;
		qd = q;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif