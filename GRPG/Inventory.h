#ifndef _INVENTORY_H                 // Prevent multiple definitions if this 
#define _INVENTORY_H                 // file is included in more than one place

#include "constants.h"
#include "InventoryItem.h"
#include <map>

class Inventory
{
private:
	int xDrawPosition, yDrawPosition;//positions for drawing the item objects
	const int maxSlotListCount = 20;
	const int inventoryColumns = 3;
	const int magicPadding = 5;
	map<int,InventoryItem*> slotList;
	InventoryItem* slot_body, slot_hand, slot_offhand;

public:
	// constructor
	Inventory();//If you don't have a .cpp, don't put ';', put '{}'
	Inventory(int x, int y);
	bool addInventoryItem(int i, InventoryItem* ii);
	bool addInventoryItem(InventoryItem* ii);
	bool removeInventoryItem(int i);
	bool hasInventoryItem(int i);

	int getXDrawPosition(){ return xDrawPosition; }
	void setXDrawPosition(int x){ xDrawPosition = x; }
	int getYDrawPosition(){ return yDrawPosition; }
	void setYDrawPosition(int x){ yDrawPosition = x; }

	InventoryItem* getInventoryItem(int i);
	map<int,InventoryItem*>* getSlotList(){ return &slotList; }
};
#endif