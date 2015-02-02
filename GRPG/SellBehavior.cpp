#include "SellBehavior.h"
#include "NPC.h"
#include "player.h"
#include "entity.h"

string SellBehavior::displayText(){
	return ("Sell 1 of " + theItem->getInventoryItem()->getItem()->getName());
}

//Purchase said item
void SellBehavior::action()
{
	player->getInventory()->removeEntityInventoryItem(theItem);
}