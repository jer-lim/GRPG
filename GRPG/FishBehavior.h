#ifndef FISHBEHAVIOR_H
#define FISHBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class FishBehavior :public Behavior
{
private:
public:
	FishBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif