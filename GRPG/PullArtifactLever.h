#ifndef PULLARTIFACTLEVER_BEHAVIOR_H
#define PULLARTIFACTLEVER_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class QuestData;
class Grpg;

class PullArtifactLeverBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	QuestData* qd;
	Grpg* grpg;
public:
	PullArtifactLeverBehavior(Player* p, Entity* e, UI* u, QuestData* q, Grpg* g){
		player = p;
		entity = e;
		ui = u;
		qd = q;
		grpg = g;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif