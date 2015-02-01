#ifndef VIEWBEHAVIOR_H
#define VIEWBEHAVIOR_H

#include "Behavior.h"
using namespace std;

class ViewBehavior:public Behavior
{
private:
	string examineText;
public:
	ViewBehavior() {
		examineText = "";
	}
	ViewBehavior(string et){
		examineText = et;
	}
	virtual ~ViewBehavior() {}
	virtual bool exists(){ return false; }
	virtual string displayText(){ return examineText; }
	virtual void action(){}
};
#endif