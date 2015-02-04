#include "EatBehavior.h"
#include "entity.h"
#include "player.h"
#include "grpg.h"

string EatBehavior::displayText(){
	return "Eat " + food->getInventoryItem()->getName();
}

void EatBehavior::action()
{
	switch (((InventoryFood*)food->getInventoryItem())->getFoodState())
	{
	case COOKED:
		player->setHealth(player->getHealth() + ((Food*)food->getInventoryItem()->getItem())->gethealth_gain());
		break;
	case DELICIOUS:
		player->setHealth(player->getHealth() + ((Food*)food->getInventoryItem()->getItem())->gethealth_gain()*FoodNS::DELICIOUS_HP_MULTIPLIER);
		break;
	}
	gamePtr->setMouseOverEntity(nullptr);
	player->getInventory()->destroyEntityInventoryItems(food, false, (Grpg*)gamePtr);
	SAFE_DELETE(food);	//delete entity
}