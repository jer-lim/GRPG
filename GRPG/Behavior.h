#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <string>
using namespace std;

class Behavior
{
private:
public:
	Behavior(){}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif