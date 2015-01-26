#ifndef _INVENTORY_ITEM_H                 // Prevent multiple definitions if this 
#define _INVENTORY_ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "item.h"

class InventoryItem
{
private:
	Item* item;
	int currentStackCount;
public:
	// constructor
	InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	~InventoryItem(){ SAFE_DELETE(item); }
	InventoryItem(Item &item, int csc){
		item = item;
		currentStackCount = csc;
	}
	Item* getItem(){ return item; }
	void setItem(Item* s){ item = s; }
	int getcurrentStackCount() { return currentStackCount; }
	void setcurrentStackCount(int i) { currentStackCount = i; }
};
#endif