#ifndef _INVENTORY_ITEM_H                 // Prevent multiple definitions if this 
#define _INVENTORY_ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include "item.h"
#include "game.h"
#include "entity.h"
#include "drawManager.h"

class InventoryItem
{
private:
	Item* item=nullptr;
	int currentStackCount=-1;
	Entity* entity;
	DrawManager* drawmanager;
public:
	void initialize(Game* gamePtr, bool anchor)
	{
		item->initializeTexture(gamePtr);
		entity = new Entity();
		entity->initialize(gamePtr, item->getSpriteWidth(), item->getspriteHeight(), item->getspriteColumns(), item->getTextureManager(), anchor);
	}
	// constructor
	InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	void destroy(){
		//drawmanager->removeObject(entity);
		item->inventoryItemDestroyed();
		item = nullptr;
		SAFE_DELETE(entity);
	}
	~InventoryItem(){ 
		destroy();
	}// don't destroy the item because it's used by multiple inventoryitems
	InventoryItem(Item *itm, int stack){
		item = itm;
		currentStackCount = stack;
	}
	Item* getItem(){ return item; }
	void setItem(Item* s){ item = s; }
	int getcurrentStackCount() { return currentStackCount; }
	void setcurrentStackCount(int i) { currentStackCount = i; }

	Entity* getEntity(){ return entity; }
	void setEntity(Entity* e){ entity = e; }
};
#endif