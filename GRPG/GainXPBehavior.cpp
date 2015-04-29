#include "GainXPBehavior.h"
#include "entity.h"
#include "UI.h"
#include "player.h"
#include "grpg.h"
#include "Boost.h"
#include "InventoryBoost.h"

string GainXPBehavior::displayText(){
	return "Gain XP from " + item->getInventoryItem()->getName();
}

void GainXPBehavior::action()
{
	((InventoryBoost*)item->getInventoryItem())->performBoost(ui, player);
	gamePtr->setMouseOverEntity(nullptr);
	
	player->getInventory()->destroyEntityInventoryItems(item, true, (Grpg*)gamePtr);
	//player->getInventory()->removeEntityInventoryItem(food, (Grpg*)gamePtr);
	//SAFE_DELETE(food);	//delete entity
}