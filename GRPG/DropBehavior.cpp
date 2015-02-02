#include "DropBehavior.h"
#include "PickupBehavior.h"
#include "player.h"

void DropBehavior::action(){
	player->getInventory()->removeEntityInventoryItem(entity);//remove from inventory
	entity->setPickupBehavior(new PickupBehavior(gamePtr,drawManager, entity, player));//change behaviors
	entity->setDropBehavior(nullptr);
	entity->setupVectorActiveBehaviors();
	entity->setX(player->getX());//set the entity to the player's position (it was previously in the inventory position)
	entity->setY(player->getY());
	entity->setAnchored(false);//and make it affected by viewport
	gamePtr->setMouseOverEntity(nullptr);
	//a player cannot drop an object without first looking at the object entity and calling its dropbehavior action, so there's no need for an if here
	//drawManager->addObject(entity,1);
	delete this;
}