#include "EatBehavior.h"
#include "entity.h"
#include "player.h"

string EatBehavior::displayText(){
	return "Eat " + food->getInventoryItem()->getItem()->getName();
}

void EatBehavior::action()
{

}