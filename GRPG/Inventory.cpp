#include "Inventory.h"

Inventory::Inventory(){
	slot_body = slot_hand = slot_offhand = InventoryItem();//Mattgic
	for (int i = 0; i < maxSlotListCount; ++i)
	{
		slotList[i] = InventoryItem();
	}
}

bool Inventory::addInventoryItem(int i, InventoryItem ii)
{
	if (i >= 0 && i < maxSlotListCount)
	{
		slotList[i] = ii;
		return true;
	}
	return false;
}

bool Inventory::removeInventoryItem(int i)
{
	if (i >= 0 && i < maxSlotListCount)
	{
		slotList[i].destroy();
		slotList[i] = InventoryItem();
		return true;
	}
	return false;
}

bool Inventory::hasInventoryItem(int i)
{
	if (i >= 0 && i < maxSlotListCount)
	{
		return slotList[i].getcurrentStackCount() != -1;
	}
	return false;
}

InventoryItem Inventory::getInventoryItem(int i)
{
	if (i >= 0 && i < maxSlotListCount)
	{
		return slotList[i];
	}
	return InventoryItem();
}