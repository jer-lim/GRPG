#ifndef MINEBEHAVIOR_H
#define MINEBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class MineBehavior :public Behavior
{
private:
public:
	MineBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif