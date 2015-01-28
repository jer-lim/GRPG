#include "Inventory.h"

Inventory::Inventory(){
	//slot_body = slot_hand = slot_offhand = InventoryItem();//Mattgic
	/*for (int i = 0; i < maxSlotListCount; ++i)
	{
		slotList[i] = InventoryItem();
	}*/
}

bool Inventory::addInventoryItem(int i, InventoryItem ii)
{
	if (slotList.size() < maxSlotListCount)
	{
		if (!hasInventoryItem(i))
		{
			slotList[i] = ii;
			return true;
		}
	}
	return false;
}

bool Inventory::addInventoryItem(InventoryItem ii)
{
	if (slotList.size() < maxSlotListCount)
	{//Find an empty spot and slot the item in
		int prevIndex = 0;
		for (std::map<int, InventoryItem>::iterator it = slotList.begin(); it != slotList.end(); ++it)
		{
			if (it->first > prevIndex + 1) {
				addInventoryItem(prevIndex + 1, ii);
				return true;
			}
			else {
				prevIndex = it->first;
			}
		}
	}
	return false;
}

bool Inventory::removeInventoryItem(int i)
{
	if (hasInventoryItem(i))
	{
		slotList[i].destroy();
		slotList.erase(i);
		return true;
	}
	return false;
}

bool Inventory::hasInventoryItem(int i)
{
	map<int, InventoryItem>::iterator it = slotList.find(i);
	if (it != slotList.end())
	{
		return true;
	}
	return false;
}

InventoryItem Inventory::getInventoryItem(int i)
{
	if (hasInventoryItem(i))
	{
		return slotList[i];
	}
	return InventoryItem();
}