//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <string>
using namespace std;

class Behavior
{
private:
public:
	Behavior(){}
	virtual ~Behavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return ""; }
	virtual void action(){}
};
#endif