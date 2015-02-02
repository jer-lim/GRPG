#include "BuyBehavior.h"
#include "NPC.h"
#include "player.h"
#include "entity.h"

string BuyBehavior::displayText(){
	return ("Buy 1 of " + theItem->getInventoryItem()->getItem()->getName());
}

//Purchase said item
void BuyBehavior::action()
{
	player->getInventory()->addEntityInventoryItem(theItem);
}