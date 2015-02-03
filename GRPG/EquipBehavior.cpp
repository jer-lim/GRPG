#include "PickupBehavior.h"
#include "EquipHandBehavior.h"
#include "player.h"

void EquipHandBehavior::action(){
	player->getInventory()->removeEntityInventoryItem(entity, nullptr);
	//entity->getInventory()->//set to hand
	entity->setPickupBehavior(nullptr);//change behaviors
	entity->setDropBehavior(nullptr);
	entity->setupVectorActiveBehaviors();
	entity->setAnchored(true);//and make it affected by viewport
	gamePtr->setMouseOverEntity(nullptr);
	//switch (player->getInventory()->addEntityInventoryItem(entity, (Grpg*)gamePtr))//add entity into inventory
	//{
	//case ADDED:
		//if add, set it as anchored - the position would have been already updated in addentity
		//moved to inventory
		//entity->setAnchored(true);
		//entity->setPickupBehavior(nullptr);//change behaviors
		//entity->setDropBehavior(new DropBehavior(gamePtr,drawManager, entity, player));
		//entity->setupVectorActiveBehaviors();
		//if (!player->getInventory()->getByDrawnManager())//if its not the inventory tab, and thus it's not supposed to be drawn now
		//drawManager->removeObject(entity);//remove from drawmanager (will be added back later when on inventory tab)
		//gamePtr->setMouseOverEntity(nullptr);
		//delete this;
		//break;
	//case MERGED:
		//if merged, we don't need the entity because 2 entities have already become 1
		//so delete itself
		//moved to inventory
		//gamePtr->setMouseOverEntity(nullptr);
		//drawManager->removeObject(entity);
		//SAFE_DELETE(entity);
		//break;
		//if it failed / partial merge, we don't touch the entity because it's still on the ground
	//case PARTIAL_MERGE:
		//break;
	//case FAILED:
		//break;
	//}
}