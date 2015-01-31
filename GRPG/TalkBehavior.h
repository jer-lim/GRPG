#ifndef TALKBEHAVIOR_H
#define TALKBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class TalkBehavior :public Behavior
{
private:
public:
	TalkBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif