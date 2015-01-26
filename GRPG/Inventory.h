#ifndef _INVENTORY_H                 // Prevent multiple definitions if this 
#define _INVENTORY_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"
#include <unordered_map>

class Inventory
{
private:
	const int maxSlotListCount = 20;
	unordered_map<int,InventoryItem> slotList;
	InventoryItem slot_body, slot_hand, slot_offhand;

public:
	// constructor
	Inventory();//If you don't have a .cpp, don't put ';', put '{}'
	bool addInventoryItem(int i, InventoryItem ii);
	bool removeInventoryItem(int i);
	bool hasInventoryItem(int i);
	InventoryItem getInventoryItem(int i);
};
#endif