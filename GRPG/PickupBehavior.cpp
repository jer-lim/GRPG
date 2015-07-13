//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "PickupBehavior.h"
#include "grpg.h"
#include "DropBehavior.h"
#include "player.h"

void PickupBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*player, collisionVector))
	{//if it is range with player
		INVENTORY_CHANGE result = player->getInventory()->addEntityInventoryItem(entity, (Grpg*)gamePtr);//add entity into inventory
		//delete this;//THIS MUST BE CALLED IF YOU INTEND TO REMOVE THE BEHAVIOR
		//deleted in destructor to account for merge condition
		if (result == INVENTORY_CHANGE::FAILED || result == INVENTORY_CHANGE::PARTIAL_MERGE)
		{
			((Grpg*)gamePtr)->getUI()->addChatText("Your inventory is full. Try dropping something to make space.");
		}
	}
	else
	{//else make player run towards it and then execute action later
		player->setVictim(entity);
		player->setNPCAction(this);
	}
}