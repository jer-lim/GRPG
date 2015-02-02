#include "Inventory.h"
#include "entity.h"

Inventory::Inventory(){
	//slot_body = slot_hand = slot_offhand = InventoryItem();//Mattgic
	/*for (int i = 0; i < maxSlotListCount; ++i)
	{
		slotList[i] = InventoryItem();
	}*/
}
Inventory::Inventory(int x,int y){
	xDrawPosition = x;
	yDrawPosition = y;
}

void Inventory::destroy()
{
	//if (!drawnByDrawManager)
	//{
	for (map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it){
		delete it->second;
		it->second = nullptr;
	}
	//}
	slotList.clear();
	SAFE_DELETE(slot_body);
	SAFE_DELETE(slot_hand);
	SAFE_DELETE(slot_offhand);
}

bool Inventory::addEntityInventoryItem(int i, Entity* ii)
{
	if (slotList.size() < maxSlotListCount)
	{
		if (!hasEntityInventoryItem(i))
		{
			int row = i / inventoryColumns + 1;
			int col = i % inventoryColumns + 1;
			ii->setX(xDrawPosition + col*ii->getInventoryItem()->getItem()->getSpriteWidth() + magicPadding);
			ii->setY(yDrawPosition + row*ii->getInventoryItem()->getItem()->getSpriteHeight() + magicPadding);
			slotList[i] = ii;
			return true;
		}
	}
	return false;
}

ITEM_ADD Inventory::addEntityInventoryItem(Entity* ii)
{
	//First check if can merge
	int result = IMPOSSIBLE;
	for (std::map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it)
	{
		result = IMPOSSIBLE;
		result =	merge(it->second, ii);
		if (result == SUCCESSFUL)
			return MERGED;//already deleted entity inside merge and no need to add item since its merged
	}
	//If incomplete / impossible, proceed
	if (result == IMPOSSIBLE || result == INCOMPLETE)
	{
		if (slotList.size() == 0)
		{//everything is empty
			addEntityInventoryItem(0, ii);
			return ADDED;
		}
		else if (slotList.size() < maxSlotListCount)
		{//Find an empty spot and slot the item in
			int prevIndex = 0;//start at index 0
			for (std::map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it)
			{
				if (it->first > prevIndex) {//check if prevIndex less than index 
					//E.G. prevIndex: 0, first: 0, failed, so prevIndex = first+1 = 1
					//but if prevIndex:0, first: 1 (first object), so add in index 0
					addEntityInventoryItem(prevIndex, ii);
					return ADDED;
				}
				else {
					prevIndex = it->first+1;
				}
			}
			//add at lastIndex
			if (addEntityInventoryItem(prevIndex, ii))
				return ADDED;
		}
	}
	if (result == IMPOSSIBLE) return FAILED;
	if (result == INCOMPLETE) return PARTIAL_MERGE;
}

bool Inventory::removeEntityInventoryItem(Entity * entity)
{
	// Iterate to element to remove it
	for (map<int,Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it){
		if (it->second == entity){
			//SAFE_DELETE(it->second);
			slotList.erase(it->first);
			return true;
		}
	}

	return false; 
}

bool Inventory::destroyEntityInventoryItem(int i)
{
	if (hasEntityInventoryItem(i))
	{
		SAFE_DELETE(slotList[i]);
		slotList[i] = nullptr;
		slotList.erase(i);
		return true;
	}
	return false;
}

bool Inventory::hasEntityInventoryItem(int i)
{
	map<int, Entity*>::iterator it = slotList.find(i);
	if (it != slotList.end())
	{
		return true;
	}
	return false;
}

Entity* Inventory::getEntityInventoryItem(int i)
{
	if (hasEntityInventoryItem(i))
	{
		return slotList[i];
	}
	return nullptr;
}

ITEM_MERGE Inventory::merge(Entity* a, Entity* b)
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
			//SAFE_DELETE(b);
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

vector<Entity* > Inventory::getVectorItems()
{
	vector<Entity*> returnValue;
	for (map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it) {
		returnValue.push_back(it->second);
	}
	return returnValue;
}