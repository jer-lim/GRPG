#ifndef COOKBEHAVIOR_H
#define COOKBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class CookBehavior :public Behavior
{
private:
public:
	CookBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif