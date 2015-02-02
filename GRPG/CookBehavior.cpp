#include "CookBehavior.h"
#include "entity.h"
#include "player.h"

string CookBehavior::displayText(){
	return "Cook " + food->getInventoryItem()->getItem()->getName();
}

void CookBehavior::action()
{
	//check proximity
	((InventoryFood*)food->getInventoryItem())->cook(player->getSkills()->at(skillNS::ID_SKILL_COOKING).getSkillLevel());
	//check food state and set behaviors
}