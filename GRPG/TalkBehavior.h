#ifndef TALKBEHAVIOR_H
#define TALKBEHAVIOR_H

#include "Behavior.h"
using namespace std;
class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;

class TalkBehavior : public Behavior
{
private:
	NPC* ii;
	UI* ui;
public:
	TalkBehavior(NPC* i, UI* u){
		ii = i;
		ui = u;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif