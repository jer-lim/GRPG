#ifndef TRADEBEHAVIOR_H
#define TRADEBEHAVIOR_H

#include "Behavior.h"
using namespace std;
class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;

class TradeBehavior :public Behavior
{
private:
	NPC* ii;
	UI* ui;
public:
	TradeBehavior(NPC* i, UI* u){
		ii = i;
		ui = u;
	}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif