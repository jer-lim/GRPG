#ifndef _INVENTORY_ITEM_H                 // Prevent multiple definitions if this 
#define _INVENTORY_ITEM_H                 // file is included in more than one place

#include "constants.h"
#include <string>
#include <sstream>
#include "item.h"
#include "game.h"
#include "entity.h"
#include "drawManager.h"

class InventoryItem
{
private:
	Item* item=nullptr;
	int currentStackCount=-1;
	string currentStackCountString;
	Entity* entity;
	DrawManager* drawmanager;
public:
	void initialize(Game* gamePtr, bool anchor)
	{
		item->initializeTexture(gamePtr);
		entity = new Entity();
		entity->initialize(gamePtr, item->getSpriteWidth(), item->getSpriteHeight(), item->getSpriteColumns(), item->getTextureManager(), anchor);
	}
	// constructor
	InventoryItem(){}//If you don't have a .cpp, don't put ';', put '{}'
	void destroy(){
		//drawmanager->removeObject(entity);
		SAFE_RELEASE(item);
		SAFE_DELETE(entity);
		drawmanager = nullptr;
	}
	~InventoryItem(){ 
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

	Entity* getEntity(){ return entity; }
	void setEntity(Entity* e){ entity = e; }
};
#endif