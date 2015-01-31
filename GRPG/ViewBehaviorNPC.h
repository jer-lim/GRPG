#ifndef VIEWBEHAVIORNPC_H
#define VIEWBEHAVIORNPC_H

#include "viewbehavior.h"
//#include "NPC.h"
using namespace std;

class NPC;//Entity imports enemy which is a subclass if NPC, so circular loop inbound
class ViewBehaviorNPC :public ViewBehavior
{
private:
	NPC* ii;
public:
	ViewBehaviorNPC(NPC* i){ 
		ii = i; 
	}
	virtual bool exists(){ return true; }
	virtual string displayText();
	virtual void action();
};
#endif