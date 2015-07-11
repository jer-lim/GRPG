#ifndef SAVECRYSTALUSE_BEHAVIOR_H
#define SAVECRYSTALUSE_BEHAVIOR_H

#include "Behavior.h"
#include "ChatData.h"
class NPC;
class UI;
using namespace std;
class Player;
class Grpg;
class ChatOption;
class Entity;

class SaveCrystalUseBehavior : public Behavior, public OptionNotifier
{
private:
	Entity* entity;
	Player* player;
	UI* ui;
	Grpg* grpg;
public:
	SaveCrystalUseBehavior(Player* p, Entity* e, UI* u, Grpg* g){
		player = p;
		entity = e;
		ui = u;
		grpg = g;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();

	virtual void action();

	virtual void optionSelected(ChatOption co);
};
#endif