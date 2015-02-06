//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef VIEWBEHAVIOR_H
#define VIEWBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class UI;

class ViewBehavior:public Behavior
{
private:
	string name;
	string examineText;
	UI* ui;
public:
	ViewBehavior() {
		examineText = "";
	}
	ViewBehavior(string n, string et, UI* u){
		name = n;
		examineText = et;
		ui = u;
	}
	virtual ~ViewBehavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText();
	virtual void action();
};
#endif