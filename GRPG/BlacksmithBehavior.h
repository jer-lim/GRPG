//  Student Name:       Matthew Lee
//  Student Number:     s10128677
// NOT IMPLEMENTED DUE TO LACK OF TIME, MEANT TO BE SMITHING

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