#include "PickupBehavior.h"
#include "DropBehavior.h"
#include "player.h"

void PickupBehavior::action(){
	VECTOR2 collisionVector;
	if (entity->collidesWith(*player, collisionVector))
	{//if it is range with player
		switch (player->getInventory()->addEntityInventoryItem(entity))//add entity into inventory
		{
		case ADDED:
			//if add, set it as anchored - the position would have been already updated in addentity
			entity->setAnchored(true);
			entity->setPickupBehavior(nullptr);//change behaviors
			entity->setDropBehavior(new DropBehavior(drawManager, entity, player));
			entity->setupVectorActiveBehaviors();
			if (!player->getInventory()->getByDrawnManager())//if its not the inventory tab, and thus it's not supposed to be drawn now
				drawManager->removeObject(entity);//remove from drawmanager (will be added back later when on inventory tab)
			delete this;
			break;
		case MERGED:
			//if merged, we don't need the entity because 2 entities have already become 1
			//so delete itself
			drawManager->removeObject(entity);
			SAFE_DELETE(entity);
			break;
		//if it failed / partial merge, we don't touch the entity because it's still on the ground
		case PARTIAL_MERGE:
			break;
		case FAILED:
			break;
		}
	}
	else
	{//else make player run towards it and then execute action later
		player->setVictim(entity);
		player->setNPCAction(this);
	}
}