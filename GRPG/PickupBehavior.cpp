#include "PickupBehavior.h"
#include "DropBehavior.h"
#include "player.h"

void PickupBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*player, collisionVector))
	{//if it is range with player
		player->getInventory()->addEntityInventoryItem(entity, (Grpg*)gamePtr);//add entity into inventory
		delete this;//THIS MUST BE CALLED IF YOU INTEND TO REMOVE THE BEHAVIOR
	}
	else
	{//else make player run towards it and then execute action later
		player->setVictim(entity);
		player->setNPCAction(this);
	}
}