#include "CookBehavior.h"
#include "entity.h"
#include "player.h"
#include "grpg.h"
#include "UI.h"

string CookBehavior::displayText(){
	return "Cook " + food->getInventoryItem()->getName();
}

void CookBehavior::action()
{
	//check proximity
	if (player->getNearStove())//this variable should be set when near a stove, and there should be a "move to stove" behavior
	{
		((InventoryFood*)food->getInventoryItem())->cook(player->getSkills()->at(skillNS::ID_SKILL_COOKING).getSkillLevel());
		//remove raw
		switch (((InventoryFood*)food->getInventoryItem())->getFoodState())
		{//check food state and set behaviors
		case COOKED:
		case DELICIOUS:
			food->setEatBehavior(new EatBehavior(gamePtr,food,player));
			break;
		}
		food->setCookBehavior(nullptr);
		food->setupVectorActiveBehaviors();
		food->update(0.1, gamePtr);
		player->getSkills()->at(skillNS::ID_SKILL_COOKING).gainXP();
		delete this;//THIS MUST BE CALLED IF YOU INTEND TO REMOVE THE BEHAVIOR
	}
	else
		((Grpg*)gamePtr)->getUI()->addChatText("You need to be near a stove to cook food.");
}