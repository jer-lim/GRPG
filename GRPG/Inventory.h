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
	const int inventoryColumns = 4;
	const int magicPadding = 5;
	map<int,Entity*> slotList;
	Entity *slot_body, *slot_hand, *slot_offhand;
	//bool drawnByDrawManager = false;
	/*
	Drawmanager x Inventory magic


	drawnByDrawManager
	For player's inventory / shopkeeper inventory, there are times when they are drawn and thts when they are
	pushed into the draw manager. At this point, if the game exits the objects will be deleted by the drawmanager.
	Thus this bool is to check if it's drawn by the drawmanager and thus not have the inventory destructor destroy
	the entities after it's deleted by the drawmanager.
	
	*/

public:
	// constructor
	Inventory();
	void destroy();
	~Inventory(){ destroy(); }
	Inventory(int x, int y);
	bool addEntityInventoryItem(int i, Entity* ii);
	ITEM_ADD addEntityInventoryItem(Entity* ii);
	bool removeEntityInventoryItem(Entity* entity);
	bool destroyEntityInventoryItem(int i);
	bool hasEntityInventoryItem(int i);
	ITEM_MERGE merge(Entity* a, Entity* b);

	int getMaxSlotCount() { return maxSlotListCount; }
	int getXDrawPosition(){ return xDrawPosition; }
	void setXDrawPosition(int x){ xDrawPosition = x; }
	int getYDrawPosition(){ return yDrawPosition; }
	void setYDrawPosition(int x){ yDrawPosition = x; }
	//bool getByDrawnManager(){ return drawnByDrawManager; }
	//void setByDrawnManager(bool b){ drawnByDrawManager = b; }

	Entity* getEntityInventoryItem(int i);
	map<int, Entity*>* getSlotList(){ return &slotList; }
	vector<Entity*> getVectorItems();

	bool hasSpaceInInventory() { return slotList.size() < maxSlotListCount; }
};
#endif