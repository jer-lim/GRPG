#ifndef TRADEBEHAVIOR_H
#define TRADEBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class TradeBehavior :public Behavior
{
private:
public:
	TradeBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif