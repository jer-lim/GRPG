#include "PickupBehavior.h"
#include "DropBehavior.h"
#include "player.h"

void PickupBehavior::action(){
	switch (player->getInventory()->addEntityInventoryItem(entity))
	{
	case ADDED:
		entity->setAnchored(true);
		entity->setPickupBehavior(nullptr);
		entity->setDropBehavior(new DropBehavior(drawManager,entity,player));
		entity->setupVectorActiveBehaviors();
		delete this;
		break;
	case MERGED:
		drawManager->removeObject(entity);
		SAFE_DELETE(entity);
		break;
	case PARTIAL_MERGE:
		break;
	case FAILED:
		break;
	}
}