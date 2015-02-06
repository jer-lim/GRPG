//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161

#ifndef VIEWBEHAVIORNPC_H
#define VIEWBEHAVIORNPC_H

#include "viewbehavior.h"
//#include "NPC.h"
using namespace std;

class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class UI;
class ViewBehaviorNPC :public ViewBehavior
{
private:
	NPC* ii;
	UI* ui;
public:
	ViewBehaviorNPC(NPC* i, UI* u){ 
		ii = i; 
		ui = u;
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};
#endif