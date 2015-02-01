#ifndef FISHBEHAVIOR_H
#define FISHBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class FishBehavior :public Behavior
{
private:
public:
	virtual ~FishBehavior() {  }
	FishBehavior();
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif