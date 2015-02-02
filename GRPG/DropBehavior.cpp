#include "DropBehavior.h"
#include "PickupBehavior.h"
#include "player.h"

void DropBehavior::action(){
	player->getInventory()->removeEntityInventoryItem(entity);
	entity->setPickupBehavior(new PickupBehavior(drawManager, entity, player));
	entity->setDropBehavior(nullptr);
	entity->setupVectorActiveBehaviors();
	entity->setX(player->getX());
	entity->setY(player->getY());
	entity->setAnchored(false);
	//drawManager->addObject(entity,1);
	delete this;
}