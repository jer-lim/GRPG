//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "BuyBehavior.h"
#include "NPC.h"
#include "player.h"
#include "entity.h"
#include "grpg.h"

string BuyBehavior::displayText(){
	return ("Buy 1 of " + theItem->getInventoryItem()->getName());
}

//Purchase said item
void BuyBehavior::action()
{
	int cost = theItem->getInventoryItem()->getCost();
	coin->getInventoryItem()->setCurrentStackCount(cost);
	if (player->getInventory()->destroyEntityInventoryItems(coin, true, grpg))
	{
		//Just create a new item and add it in - (c) Matt
		//If it returns MERGE, FAILED and PARTIAL_MERGE delete
		InventoryItem* x = theItem->getInventoryItem()->clone();
		Entity* newObj = new Entity();
		newObj->initialize(grpg, x, false);

		int result = player->getInventory()->addEntityInventoryItem(newObj, grpg);
		if (result == FAILED || result == PARTIAL_MERGE)
		{
			//drop the rest on the grond
			newObj->setX(player->getX());
			grpg->getDrawManager()->addObject(newObj);
			newObj->setY(player->getY());
		}
		else if (result == MERGED)
		{
			//Matt says it should already be handled
			//delete newObj;
		}
		//Add a sellBehavior
		newObj->sellBehavior = new SellBehavior(player, newObj, grpg);
		newObj->setupVectorActiveBehaviors();
	}
	else
	{
		//Can't afford
		grpg->getUI()->addChatText("You can't afford that " + theItem->getInventoryItem()->getName());
	}
}