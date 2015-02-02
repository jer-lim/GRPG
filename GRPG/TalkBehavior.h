#ifndef TALKBEHAVIOR_H
#define TALKBEHAVIOR_H

#include "Behavior.h"
using namespace std;
class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;
class Player;
class Entity;

class TalkBehavior : public Behavior
{
private:
	NPC* ii;
	UI* ui;
	Player* thePlayer;
	Entity* entity; //The entity to talk to
public:
	virtual ~TalkBehavior() {}
	TalkBehavior(NPC* i, UI* u, Player* p, Entity* e){
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