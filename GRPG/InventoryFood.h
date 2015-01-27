#ifndef _INVENTORY_FOOD_H                 // Prevent multiple definitions if this 
#define _INVENTORY_FOOD_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"

class InventoryFood : public InventoryItem
{
private:
	bool isCooked, isDelicious;
public:
	// constructor
	InventoryFood():InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	InventoryFood(Item &item, int csc,bool isC,bool isD)
		:InventoryItem(item, csc)
	{
		isCooked = isC;
		isDelicious = isD;
	}
	bool isDelicious(){ return isCooked; }
	void setisDelicious(bool s){ isCooked = s; }
	bool isDelicious(){ return isDelicious; }
	void setisDelicious(bool s){ isDelicious = s; }
};
#endif