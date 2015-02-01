#include "PickupBehavior.h"
#include "DropBehavior.h"
#include "player.h"

void PickupBehavior::action(){
	switch (player->getInventory()->addEntityInventoryItem(entity))
	{
	case ADDED:
		entity->setPickupBehavior(nullptr);
		entity->setDropBehavior(new DropBehavior(entity));
		entity->setupVectorActiveBehaviors();
		break;
	case MERGED:
		drawManager->removeObject(entity);
		SAFE_DELETE(entity);
		break;
	}
}