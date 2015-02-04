#include "SellBehavior.h"
#include "NPC.h"
#include "player.h"
#include "entity.h"
#include "grpg.h"
#include "itemLoader.h"

string SellBehavior::displayText(){
	return ("Sell 1 of " + theItem->getInventoryItem()->getName());
}

//Purchase said item
void SellBehavior::action()
{
	int cost = theItem->getInventoryItem()->getSellingCost();
	Entity* itemClone = new Entity();
	itemClone->initialize(grpg, theItem->getInventoryItem()->clone(), true);
	itemClone->getInventoryItem()->setCurrentStackCount(1);
	//I have NO IDEA why  I have to do this
	Grpg* anotherPointer = grpg;
	player->getInventory()->destroyEntityInventoryItems(itemClone, true, grpg);
	grpg = anotherPointer;
	//Spawn coins for the player
	InventoryItem* x = new InventoryItem(grpg->getItemLoader()->getItem(0), cost);
	grpg = anotherPointer;
	Entity* newObj = new Entity();
	newObj->initialize(grpg, x, true);

	//If it returns MERGE, FAILED and PARTIAL_MERGE delete
	int result = player->getInventory()->addEntityInventoryItem(newObj, grpg);
	if (result == FAILED || result == PARTIAL_MERGE)
	{
		//drop the remaining stuff onto the ground
		newObj->setX(player->getX());
		newObj->setY(player->getY());
		grpg->getDrawManager()->addObject(newObj);
	}
	else if (result == MERGED)
	{
		//Don't need to delete, inventory does it for me
		//delete newObj;
	}
}