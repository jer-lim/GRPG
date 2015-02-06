//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef VIEWBEHAVIORITEM_H
#define VIEWBEHAVIORITEM_H

#include "viewbehavior.h"
#include "InventoryItem.h"
using namespace std;
class UI;

class ViewBehaviorItem :public ViewBehavior
{
private:
	InventoryItem* ii;
	UI* ui;
public:
	ViewBehaviorItem(InventoryItem* i, UI* u){
		ii = i;
		ui = u;
	}
	virtual bool exists(){ return true; }
	virtual string displayText(){ return "Examine "+ii->getName(); }
	virtual void action();
};
#endif