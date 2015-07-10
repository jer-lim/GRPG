//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "Inventory.h"
#include "entity.h"
#include "grpg.h"
#include "DropBehavior.h"
#include "PickupBehavior.h"
#include "InventoryBoost.h"

Inventory::Inventory(){
	slot_body = -1;
	slot_hand = -1;
	slot_offhand = -1;
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
	//slots are just references to stuff inside the inventory as of now
	///SAFE_DELETE(slot_body);
	//SAFE_DELETE(slot_hand);
	//SAFE_DELETE(slot_offhand);
}

bool Inventory::addEntityInventoryItem(int i, Entity* ii)
{
	if (slotList.size() < maxSlotListCount)
	{
		if (!hasEntityInventoryItem(i))
		{
			if (ii->getInventoryItem()->getType() == "INVENTORYEQUIPMENT")
			{//is equipment
				Equipment* eq = (Equipment*)ii->getInventoryItem()->getItem();
				if (eq->occupiesBody())
				{//is body armor
					if (getSlotBody() == nullptr || getSlotBody()->getInventoryItem()->getCost() < ii->getInventoryItem()->getCost())
					{//if new armor is better (more expensive)
						slot_body = i;
					}
				}
				else if (eq->occupiesHand())
				{
					if (eq->occupiesOffhand())
					{//normal weapon
						if (getSlotHand() == nullptr || getSlotHand()->getInventoryItem()->getCost() < ii->getInventoryItem()->getCost())
						{//if new weapon is better (more expensive)
							slot_hand = i;
						}
					}
				}
				else
				{//shield
					if (getSlotOffHand() == nullptr || getSlotOffHand()->getInventoryItem()->getCost() < ii->getInventoryItem()->getCost())
					{//if new shield is better (more expensive)
						slot_offhand = i;
					}
				}
			}
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

INVENTORY_CHANGE Inventory::addEntityInventoryItem(Entity* ii, Grpg* gamePtr, bool noDelete)
{
	//First check if can merge
	int result = IMPOSSIBLE;
	for (std::map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it)
	{
		result = IMPOSSIBLE;
		result = merge(it->second, ii);
		if (result == SUCCESSFUL)
		{
			//result = MERGED;
			if (gamePtr != nullptr)
			{
				gamePtr->setMouseOverEntity(nullptr);
				gamePtr->getDrawManager()->removeObject(ii);
				if (!noDelete)
				{
					SAFE_DELETE(ii);
				}
			}
			return MERGED;
			//return MERGED;//already deleted entity inside merge and no need to add item since its merged
		}
	}
	//If incomplete / impossible, proceed
	if (result == IMPOSSIBLE || result == INCOMPLETE)
	{
		if (slotList.size() == 0)
		{//everything is empty
			addEntityInventoryItem(0, ii);
			result = SUCCESSFUL;
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
					result = SUCCESSFUL;
					break;
				}
				else {
					prevIndex = it->first+1;
				}
			}
			//add at lastIndex
			if (result != SUCCESSFUL)
			{
				if (addEntityInventoryItem(prevIndex, ii))
					result = SUCCESSFUL;
			}
		}
	}
	if (result == SUCCESSFUL) {
		//put add drop behavior
		if (gamePtr != nullptr)
		{
			ii->setAnchored(true);
			ii->setPickupBehavior(nullptr);//change behaviors
			ii->setDropBehavior(new DropBehavior(gamePtr, gamePtr->getDrawManager(), ii, gamePtr->getPlayer()));
			ii->setupVectorActiveBehaviors();
			//if (!player->getInventory()->getByDrawnManager())//if its not the inventory tab, and thus it's not supposed to be drawn now
			gamePtr->getDrawManager()->removeObject(ii);//remove from drawmanager (will be added back later when on inventory tab)
			gamePtr->setMouseOverEntity(nullptr);
		}
		return SUCCESS;
	}
	/*else if (result == MERGED)
	{
		if (gamePtr != nullptr)
		{
			gamePtr->setMouseOverEntity(nullptr);
			gamePtr->getDrawManager()->removeObject(ii);
			SAFE_DELETE(ii);
		}
		return MERGED;
	}*/
	else if (result == IMPOSSIBLE) { return FAILED; }
	else if (result == INCOMPLETE) { return PARTIAL_MERGE; }
}

bool Inventory::removeEntityInventoryItem(Entity * entity, Grpg* gamePtr)
{
	// Iterate to element to remove it
	for (map<int,Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it){
		if (it->second == entity){
			//add pickup behavior
			if (gamePtr != nullptr)
			{
				entity->setPickupBehavior(new PickupBehavior(gamePtr, gamePtr->getDrawManager(), entity, gamePtr->getPlayer()));//change behaviors
				entity->setDropBehavior(nullptr);
				entity->setupVectorActiveBehaviors();
				entity->setX(gamePtr->getPlayer()->getX());//set the entity to the player's position (it was previously in the inventory position)
				entity->setY(gamePtr->getPlayer()->getY());
				entity->setAnchored(false);//and make it affected by viewport
				gamePtr->setMouseOverEntity(nullptr);
			}
			//SAFE_DELETE(it->second);
			unequip(it->first);
			slotList.erase(it->first);
			return true;
		}
	}

	return false; 
}
bool Inventory::removeEntityInventoryItem(int i)
{
	if (hasEntityInventoryItem(i))
	{
		unequip(i);
		slotList[i] = nullptr;
		slotList.erase(i);
		return true;
	}
	return false;
}

int Inventory::removeEntityInventoryItems(Entity* entity, bool stackCount, vector<Entity*>* removedItems, Grpg* gamePtr)
{
	int totalStackCount = 0, goalStackCount = entity->getInventoryItem()->getCurrentStackCount();
	//quickfix
	vector<int> vector_indexes;
	for (map<int, Entity*>::iterator it = slotList.begin(); it != slotList.end(); ++it){
		if (it->second->getInventoryItem()->getItem() == entity->getInventoryItem()->getItem()){//if items are the same
			if (it->second->getInventoryItem()->getType() == entity->getInventoryItem()->getType())//if type of inventoryitem is the same
			{
				if (it->second->getInventoryItem()->getType() == "INVENTORYFOOD")
				{
					if (((InventoryFood*)it->second->getInventoryItem())->getFoodState() != ((InventoryFood*)entity->getInventoryItem())->getFoodState())
						continue;//different food state means different foods
				}
				else if (it->second->getInventoryItem()->getType() == "INVENTORYEQUIPMENT")
				{
					if (((InventoryEquipment*)it->second->getInventoryItem())->getSmithingMaterial() != ((InventoryEquipment*)entity->getInventoryItem())->getSmithingMaterial())
						continue;//different smithing materials means different equipment
				}
				//else, we don't need to check alr
				totalStackCount += it->second->getInventoryItem()->getCurrentStackCount();
				vector_indexes.push_back(it->first);
				removedItems->push_back(it->second);
				if (stackCount && totalStackCount >= goalStackCount)
					break;//we have enough stuff to remove the items
			}
		}
	}

	if (!stackCount)
	{//if ignore stackCount, just set the goalStackcount to the totalStackcount so everything is removed;
		goalStackCount = totalStackCount;
	}

	if (removedItems->size() > 0 && totalStackCount >= goalStackCount)
	{//it's got stuff and sufficient stackCount to delete shit
		int l = removedItems->size();
		for (int i = 0, l = removedItems->size(); i < l; ++i)
		{
			int stackCount = removedItems->at(i)->getInventoryItem()->getCurrentStackCount();
			if (goalStackCount >= stackCount)
			{//complete removal of entity
				goalStackCount -= stackCount;//decrement goalStackCount for progression
				//since it's a complete removal, thr is no setting of the stackcount
				//Loop through map and delete item//slotList.erase();
				//removeEntityInventoryItem(entity, nullptr);//doesn't work because the entity ptr changes AND I DON'T FUCKING KNOW WHY FUCK THIS FUCK  FUCKEVERYTIG
				if (gamePtr != nullptr)
				{
					entity->setPickupBehavior(new PickupBehavior(gamePtr, gamePtr->getDrawManager(), entity, gamePtr->getPlayer()));//change behaviors
					entity->setDropBehavior(nullptr);
					entity->setupVectorActiveBehaviors();
					entity->setX(gamePtr->getPlayer()->getX());//set the entity to the player's position (it was previously in the inventory position)
					entity->setY(gamePtr->getPlayer()->getY());
					entity->setAnchored(false);//and make it affected by viewport
					gamePtr->setMouseOverEntity(nullptr);
				}
				removeEntityInventoryItem(vector_indexes.at(i));
			}
			else
			{//partial removal of entity
				//just decrease the stack count since it's a "partial remove"
				removedItems->at(i)->getInventoryItem()->setCurrentStackCount(removedItems->at(i)->getInventoryItem()->getCurrentStackCount() - goalStackCount);
				return i;
			}
		}
		return l;
	}
	else
	{
		removedItems->clear();
		return -1;
	}
	return -1;
}

bool Inventory::destroyEntityInventoryItem(int i)
{
	if (hasEntityInventoryItem(i))
	{
		unequip(i);
		SAFE_DELETE(slotList[i]);
		slotList[i] = nullptr;
		slotList.erase(i);
		return true;
	}
	return false;
}

bool Inventory::destroyEntityInventoryItems(Entity* entity, bool stackCount, Grpg* gamePtr)
{
	vector<Entity*> removedItems;
	int removeLength = removeEntityInventoryItems(entity, stackCount, &removedItems, gamePtr);
	//delete all removeditems based on what has been removed from inventory
	for (int i = 0; i < removeLength; ++i)
	{
		delete removedItems.at(i);
		removedItems.at(i) = nullptr;
	}
	return removedItems.size() > 0;
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

string Inventory::getInventoryString()
{
	stringstream result;
	result << "{";
	vector<Entity*> inventoryItems = getVectorItems();
	for (vector<Entity*>::iterator i = inventoryItems.begin(); i != inventoryItems.end(); i++)
	{
		Entity* theItem = *i;
		InventoryItem* theInventoryItem = theItem->getInventoryItem();
		if (theInventoryItem->getType() == "INVENTORYFOOD")
		{
			result << "F";
			result << theInventoryItem->getItem()->getID() << ":";
			result << theInventoryItem->getCurrentStackCount() << ":";
			result << ((InventoryFood*)theInventoryItem)->getFoodState();
		}
		else if (theInventoryItem->getType() == "INVENTORYEQUIPMENT")
		{
			result << "E";
			result << theInventoryItem->getItem()->getID() << ":";
			result << theInventoryItem->getCurrentStackCount() << ":";
			result << ((InventoryEquipment*)theInventoryItem)->getSmithingMaterial()->getID();
		}
		else if (theInventoryItem->getType() == "INVENTORYBOOST")
		{
			result << "B";
			result << theInventoryItem->getItem()->getID() << ":";
			result << theInventoryItem->getCurrentStackCount() << ":";
		}
		else
		{
			result << theInventoryItem->getItem()->getID() << ":";
			result << theInventoryItem->getCurrentStackCount() << ":";
		}
		result << ",";
	}
	result << "}";
	return result.str();
}

void Inventory::loadInventoryString(string s, ItemLoader* itemLoader, Grpg* grpg)
{
	//Remove all inventory items
	vector<Entity*> inventoryItems = getVectorItems();
	for (vector<Entity*>::iterator i = inventoryItems.begin(); i != inventoryItems.end(); i++)
	{
		Entity* theItem = *i;

		Entity* itemClone = new Entity();
		itemClone->initialize(grpg, theItem->getInventoryItem()->clone(), true);

		vector<Entity*> entitiesToDelete;
		int size;
		size = removeEntityInventoryItems(itemClone, true, &entitiesToDelete, grpg);
		delete itemClone;

		for (int i = 0; i < size; ++i)
		{
			delete entitiesToDelete.at(i);
			entitiesToDelete[i] = nullptr;
		}
	}

	//Load items from string into inventory
	s.erase(s.begin());
	s.erase(s.end() - 1);

	InventoryItem* iiPtr;
	vector<string> vector_itemsData = String_Functions::split(s, ',');
	for (int i = 0, l = vector_itemsData.size(); i < l; ++i)
	{
		vector<string> vector_drop = String_Functions::split(vector_itemsData[i], ':');
		if (vector_drop[0].at(0) == 'F')
		{//Food "id:count:food_state"
			vector_drop[0].erase(0, 1);
			iiPtr = new InventoryFood(itemLoader->getItem(atoi(vector_drop[0].c_str())), atoi(vector_drop[1].c_str()),
				static_cast<FOOD_STATE>(atoi(vector_drop[2].c_str())));
		}
		else if (vector_drop[0].at(0) == 'E')
		{//Equipment "id:count:smithing_material"
			vector_drop[0].erase(0, 1);
			iiPtr = new InventoryEquipment(itemLoader->getItem(atoi(vector_drop[0].c_str())),
				atoi(vector_drop[1].c_str()),
				(Smithing_Material*)itemLoader->getItem(atoi(vector_drop[2].c_str())));
		}
		else if (vector_drop[0].at(0) == 'B')
		{//Boost item "id:count"
			vector_drop[0].erase(0, 1);
			iiPtr = new InventoryBoost(itemLoader->getItem(atoi(vector_drop[0].c_str())),
				atoi(vector_drop[1].c_str()));
		}
		else
		{//Normal item "id:count"
			iiPtr = new InventoryItem(itemLoader->getItem(atoi(vector_drop[0].c_str())),
				atoi(vector_drop[1].c_str()));
		}
		
		//Just create a new item and add it in - (c) Matt
		//If it returns MERGE, FAILED and PARTIAL_MERGE delete
		Entity* newObj = new Entity();
		newObj->initialize(grpg, iiPtr, false);

		addEntityInventoryItem(newObj, grpg);
	}
}