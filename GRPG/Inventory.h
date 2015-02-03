#ifndef _INVENTORY_H                 // Prevent multiple definitions if this 
#define _INVENTORY_H                 // file is included in more than one place

#include "constants.h"
#include <map>
#include <vector>

class PickupBehavior;
class DropBehavior;
class Grpg;
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

public:
	// constructor
	Inventory();
	void destroy();
	~Inventory(){ destroy(); }
	Inventory(int x, int y);
	/**
	Add an item into the inventory by index. 
	*/
	bool addEntityInventoryItem(int i, Entity* ii);
	/**
	Add an item into the inventory. Removes pickup behavior Adds drop behavior to entity if gamePtr is not a nullptr, as well as removes from drawManager if needed. 
	The entity is also set to anchored if needed.
	*/
	ITEM_ADD addEntityInventoryItem(Entity* ii, Grpg* gamePtr);// = nullptr);
	/**
	Remove an item from the inventory by entity ptr (ptr of an entity obj in the inventory). 
	Removes drop behavior and adds pickup behavior, positions the entity at the player's location and unanchors it.
	*/
	bool removeEntityInventoryItem(Entity* entity, Grpg* gamePtr);
	/**
	Delete items using an entity that contains the data of item to be removed, and how much (stackCount).
	If stackCount is set to true, entity are deleted based on stackCount.
	If the total stackCount in the inventory of all copies of the item is sufficient to meet the requirement, it will put all removed items into the vector ptr and return true;
	If the total stackCount in the inventory is insufficient to meet the removed requirement, it will not remove anything and will clear the vector and return false;
	*/
	bool removeEntityInventoryItems(Entity* entity, bool stackCount, vector<Entity*>* removedItems, Grpg* gamePtr=nullptr);
	/**
	Destroy an entity inventory item completely by index.
	*/
	bool destroyEntityInventoryItem(int i);
	/**
	Delete items using an entity that contains the data of item to be removed, and how much (stackCount).
	If stackCount is set to true, entity are deleted based on stackCount.
	If the total stackCount in the inventory of all copies of the item is sufficient to meet the requirement, it will delete all items and return true.
	If the total stackCount in the inventory is insufficient to meet the removed requirement, it will not delete anything and return false.
	JUST USE REMOVE THEN DELETE SIMPLE SHIT
	*/
	bool destroyEntityInventoryItems(Entity* entity, bool stackCount, Grpg* gamePtr = nullptr);
	bool hasEntityInventoryItem(int i);
	/**
	Attempt to merge 2 entities using stack count. a is usually an entity that is already in the inventory, while b is an 
	entity that is not yet added into the inventory.
	*/
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

	Entity* getSlotHand(){ return slot_hand; }
	Entity* getSlotOffHand(){ return slot_offhand; }
	Entity* getSlotBody(){ return slot_body; }

	bool hasSpaceInInventory() { return slotList.size() < maxSlotListCount; }
};
#endif