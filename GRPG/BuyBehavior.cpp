#include "BuyBehavior.h"
#include "NPC.h"
#include "player.h"
#include "entity.h"
#include "grpg.h"

string BuyBehavior::displayText(){
	return ("Buy 1 of " + theItem->getInventoryItem()->getItem()->getName());
}

//Purchase said item
void BuyBehavior::action()
{
	int cost = theItem->getInventoryItem()->getCost();
	int totalCoin = coin->getInventoryItem()->getCurrentStackCount();
	if (cost <= totalCoin)
	{
		//Just create a new item and add it in - (c) Matt
		//If it returns MERGE, FAILED and PARTIAL_MERGE delete
		InventoryItem* x = theItem->getInventoryItem()->clone();
		Entity* newObj = new Entity();
		newObj->initialize(grpg, x, true);

		int result = player->getInventory()->addEntityInventoryItem(newObj);
		coin->getInventoryItem()->setCurrentStackCount(totalCoin - cost);
		if (result == FAILED || result == PARTIAL_MERGE)
		{
			//drop the rest on the grond
			newObj->setX(player->getX());
			grpg->getDrawManager()->addObject(newObj);
			newObj->setY(player->getY());
		}
		else if (result == MERGED)
		{
			delete newObj;
		}
	}
}