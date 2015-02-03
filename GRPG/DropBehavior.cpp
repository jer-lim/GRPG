#include "DropBehavior.h"
#include "PickupBehavior.h"
#include "player.h"

void DropBehavior::action(){
	drawManager->addObject(entity, 1);
	player->getInventory()->removeEntityInventoryItem(entity,(Grpg*)gamePtr);//remove from inventory
	//DO NOT PUT ANY THING AFTER THIS LINE BECAUSE THE BEHAVIOR IS ALREADY DELETED
}