#ifndef _INVENTORY_H                 // Prevent multiple definitions if this 
#define _INVENTORY_H                 // file is included in more than one place

#include "constants.h"
#include "Entity.h"
#include <map>

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
	Inventory();//If you don't have a .cpp, don't put ';', put '{}'
	void destroy(){
		for (map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it){
			delete it->second;
			it->second = nullptr;
		}
		slotList.clear();
		SAFE_DELETE(slot_body);
		SAFE_DELETE(slot_hand);
		SAFE_DELETE(slot_offhand);
	}
	~Inventory(){ destroy(); }
	Inventory(int x, int y);
	bool addEntityInventoryItem(int i, Entity* ii);
	bool addEntityInventoryItem(Entity* ii);
	bool destroyEntityInventoryItem(int i);
	bool hasEntityInventoryItem(int i);
	ITEM_MERGE merge(Entity* a, Entity* b)
	{//merge in terms of stackcount
		if (a->getInventoryItem()->getItem() != b->getInventoryItem()->getItem() || 
			a->getInventoryItem()->getItem()->getMaxStackCount() <= a->getInventoryItem()->getCurrentStackCount())
		{//a is already filled or different item
			return IMPOSSIBLE;
		}
		else {
			int totalStack = a->getInventoryItem()->getCurrentStackCount() + b->getInventoryItem()->getCurrentStackCount();
			if (totalStack <= a->getInventoryItem()->getItem()->getMaxStackCount())
			{//enough space for complete merge
				a->getInventoryItem()->setCurrentStackCount(totalStack);
				SAFE_DELETE(b);//b should have already been removed from draw manager before adding into inventory
				return SUCCESSFUL;
			}
			else
			{
				a->getInventoryItem()->setCurrentStackCount(a->getInventoryItem()->getItem()->getMaxStackCount());
				totalStack -= a->getInventoryItem()->getItem()->getMaxStackCount();
				b->getInventoryItem()->setCurrentStackCount(totalStack);
				return INCOMPLETE;
			}
		}
	}

	int getXDrawPosition(){ return xDrawPosition; }
	void setXDrawPosition(int x){ xDrawPosition = x; }
	int getYDrawPosition(){ return yDrawPosition; }
	void setYDrawPosition(int x){ yDrawPosition = x; }

	Entity* getEntityInventoryItem(int i);
	map<int, Entity*>* getSlotList(){ return &slotList; }
};
enum ITEM_MERGE { IMPOSSIBLE, INCOMPLETE, SUCCESSFUL };
#endif