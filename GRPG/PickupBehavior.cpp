#include "PickupBehavior.h"
#include "DropBehavior.h"
#include "player.h"

void PickupBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*player, collisionVector))
	{//if it is range with player
		player->getInventory()->addEntityInventoryItem(entity, (Grpg*)gamePtr);//add entity into inventory
		//DO NOT PUT ANY THING AFTER THIS LINE BECAUSE THE BEHAVIOR IS ALREADY DELETED
	}
	else
	{//else make player run towards it and then execute action later
		player->setVictim(entity);
		player->setNPCAction(this);
	}
}