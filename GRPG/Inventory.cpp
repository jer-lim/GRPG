#include "Inventory.h"

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

bool Inventory::addEntityInventoryItem(Entity* ii)
{
	if (slotList.size() == 0)
	{//everything is empty
		addEntityInventoryItem(0, ii);
	}
	else if (slotList.size() < maxSlotListCount)
	{//Find an empty spot and slot the item in
		int prevIndex = 0;
		for (std::map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it)
		{
			if (it->first > prevIndex + 1) {
				addEntityInventoryItem(prevIndex + 1, ii);
				return true;
			}
			else {
				prevIndex = it->first;
			}
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