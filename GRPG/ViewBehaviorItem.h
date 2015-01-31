#ifndef VIEWBEHAVIORITEM_H
#define VIEWBEHAVIORITEM_H

#include "viewbehavior.h"
#include "InventoryItem.h"
using namespace std;

class ViewBehaviorItem :public ViewBehavior
{
private:
	InventoryItem* ii;
public:
	ViewBehaviorItem(InventoryItem* i){
		ii = i;
	}
	virtual bool exists(){ return true; }
	virtual string displayText(){ return "Examine "+ii->getItem()->getName(); }
	virtual void action(){ 
		//Type stuff into the chat to say the description
		//ii->getItem()->getDescription();
	}
};
#endif