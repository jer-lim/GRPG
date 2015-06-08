#ifndef RIFTENTER_BEHAVIOR_H
#define RIFTENTER_BEHAVIOR_H

#include "Behavior.h"
#include "entity.h"
class NPC;
class UI;
using namespace std;
class Player;
class Grpg;

class RiftEnterBehavior : public Behavior
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	Grpg* grpg;
public:
	RiftEnterBehavior(Player* p, Entity* e, UI* u, Grpg* g){
		player = p;
		entity = e;
		ui = u;
		grpg = g;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();
};
#endif