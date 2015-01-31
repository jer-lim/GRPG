#ifndef VIEWBEHAVIOR_H
#define VIEWBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class ViewBehavior:public Behavior
{
private:
public:
	ViewBehavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif