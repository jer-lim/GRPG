#ifndef _INVENTORY_H                 // Prevent multiple definitions if this 
#define _INVENTORY_H                 // file is included in more than one place

#include "constants.h"
#include <map>
#include <vector>

class Entity;

class Inventory
{
private:
	int xDrawPosition, yDrawPosition;//positions for drawing the item objects
	const int maxSlotListCount = 20;
	const int inventoryColumns = 3;
	const int magicPadding = 5;
	map<int,Entity*> slotList;
	Entity *slot_body, *slot_hand, *slot_offhand;

public:
	// constructor
	Inventory();
	void destroy();
	~Inventory(){ destroy(); }
	Inventory(int x, int y);
	bool addEntityInventoryItem(int i, Entity* ii);
	ITEM_ADD addEntityInventoryItem(Entity* ii);
	bool destroyEntityInventoryItem(int i);
	bool hasEntityInventoryItem(int i);
	ITEM_MERGE merge(Entity* a, Entity* b);

	int getXDrawPosition(){ return xDrawPosition; }
	void setXDrawPosition(int x){ xDrawPosition = x; }
	int getYDrawPosition(){ return yDrawPosition; }
	void setYDrawPosition(int x){ yDrawPosition = x; }

	Entity* getEntityInventoryItem(int i);
	map<int, Entity*>* getSlotList(){ return &slotList; }
	vector<Entity*> getVectorItems();
};
#endif