//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#ifndef _INVENTORY_ITEM_H                 // Prevent multiple definitions if this 
#define _INVENTORY_ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include <sstream>
#include "item.h"
#include "game.h"
//#include "entity.h"
#include "drawManager.h"

class InventoryItem
{
protected:
	Item* item=nullptr;
	int currentStackCount=-1;
	string currentStackCountString;
	//Entity* entity = nullptr;
	DrawManager* drawmanager;
	//To mark whether the initalize code has been called, and thus whether the item should be SAFE_RELEASED.
	//The initalize code may not be called immediately if the item is created but not immediately shown
	//or assigned to an entity (i.e. shop items or a quest reward)
	bool initalized;
public:
	void initialize(Game* gamePtr)
	{
		item->initializeTexture(gamePtr);
		initalized = true;
	}
	// constructor
	InventoryItem()
	{
		initalized = false;
	}

	virtual void destroy(){
		//drawmanager->removeObject(entity);
		if (initalized)
		{
			SAFE_RELEASE(item);
		}
		//SAFE_DELETE(entity);
		drawmanager = nullptr;
	}
	virtual ~InventoryItem(){ 
		destroy();
	}// don't destroy the item because it's used by multiple inventoryitems
	InventoryItem(Item *itm, int stack){
		item = itm;
		setCurrentStackCount(stack);
	}
	Item* getItem(){ return item; }
	void setItem(Item* s){ item = s; }
	int getCurrentStackCount() { return currentStackCount; }
	void setCurrentStackCount(int i) {
		currentStackCount = i; 
		stringstream ss;
		ss << currentStackCount;
		currentStackCountString = ss.str();
	}
	string getCurrentStackCountString() { return currentStackCountString; }

	virtual string getType() { return "INVENTORYITEM"; }

	virtual InventoryItem* clone() {
		return new InventoryItem(*this);
	}

	virtual int getCost(){
		return item->getCost();
	}

	virtual int getSellingCost(){
		return item->getSellingCost();
	}

	virtual string getName()
	{
		return item->getName() + "(" + currentStackCountString + ")";
	}
	//Entity* getEntity(){ return entity; }
	//void setEntity(Entity* e){ entity = e; }
};
#endif