#ifndef TRADEBEHAVIOR_H
#define TRADEBEHAVIOR_H

#include "Behavior.h"
using namespace std;
class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;
class Player;
class Entity;

class TradeBehavior :public Behavior
{
private:
	NPC* ii;
	UI* ui;
	Player* thePlayer;
	Entity* entity; //The entity to trade with to
public:
	TradeBehavior(NPC* i, UI* u, Player* p, Entity* e){
		ii = i;
		ui = u;
		thePlayer = p;
		entity = e;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif