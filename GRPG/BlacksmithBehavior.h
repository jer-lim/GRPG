#ifndef BLACKSMITHBEHAVIOR_H
#define BLACKSMITHBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class BlacksmithBehavior :public Behavior
{
private:
public:
	BlacksmithBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif