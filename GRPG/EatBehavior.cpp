// Module: Games Programming
// Assignment 2: GRPG
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "EatBehavior.h"
#include "entity.h"
#include "player.h"
#include "grpg.h"

string EatBehavior::displayText(){
	return "Eat " + food->getInventoryItem()->getName();
}

void EatBehavior::action()
{
	if (!food->getAnchored()) //Not anchored = not in inventory
	{
		((Grpg*)gamePtr)->getUI()->addChatText("You need to pick that up first.");
		return;
	}
	switch (((InventoryFood*)food->getInventoryItem())->getFoodState())
	{
	case COOKED:
		player->setHealth(player->getHealth() + ((Food*)food->getInventoryItem()->getItem())->gethealth_gain());
		break;
	case DELICIOUS:
		player->setHealth(player->getHealth() + ((Food*)food->getInventoryItem()->getItem())->gethealth_gain()*FoodNS::DELICIOUS_HP_MULTIPLIER);
		break;
	}
	if (player->getHealth() > player->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel())
	{
		player->setHealth(player->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel());
	}
	gamePtr->setMouseOverEntity(nullptr);
	InventoryFood* foodClone = ((InventoryFood*)food->getInventoryItem())->clone();
	foodClone->setCurrentStackCount(1);
	Entity* itemClone = new Entity();
	itemClone->initialize(gamePtr, foodClone, true);
	player->getInventory()->destroyEntityInventoryItems(itemClone, true, (Grpg*)gamePtr);
	//player->getInventory()->removeEntityInventoryItem(food, (Grpg*)gamePtr);
	//SAFE_DELETE(food);	//delete entity
}