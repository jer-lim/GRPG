#include "CookBehavior.h"
#include "entity.h"
#include "player.h"

string CookBehavior::displayText(){
	return "Cook " + food->getInventoryItem()->getItem()->getName();
}

void CookBehavior::action()
{
	//check proximity
	if (gamePtr->getNearStove())
		((InventoryFood*)food->getInventoryItem())->cook(player->getSkills()->at(skillNS::ID_SKILL_COOKING).getSkillLevel());
	//remove raw
	food->setCookBehavior(nullptr);
	switch (((InventoryFood*)food->getInventoryItem())->getFoodState())
	{//check food state and set behaviors
	case COOKED:
	case DELICIOUS:
		food->setEatBehavior(new EatBehavior(gamePtr,food,player));
		break;
	}
	food->setupVectorActiveBehaviors();
}