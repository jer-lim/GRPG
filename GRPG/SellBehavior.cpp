#include "SellBehavior.h"
#include "NPC.h"
#include "player.h"
#include "entity.h"
#include "grpg.h"
#include "itemLoader.h"

string SellBehavior::displayText(){
	return ("Sell 1 of " + theItem->getInventoryItem()->getItem()->getName());
}

//Purchase said item
void SellBehavior::action()
{
	int cost = theItem->getInventoryItem()->getSellingCost();
	player->getInventory()->removeEntityInventoryItem(theItem);
	//Spawn coins for the player
	InventoryItem* x = new InventoryItem(grpg->getItemLoader()->getItem(0), cost);
	Entity* newObj = new Entity();
	newObj->initialize(grpg, x, true);

	//If it returns MERGE, FAILED and PARTIAL_MERGE delete
	int result = player->getInventory()->addEntityInventoryItem(newObj);
	if (result == FAILED || result == PARTIAL_MERGE)
	{
		//drop the remaining stuff onto the ground
		newObj->setX(player->getX());
		newObj->setY(player->getY());
		grpg->getDrawManager()->addObject(newObj);
	}
	else if (result == MERGED)
	{
		delete newObj;
	}
}