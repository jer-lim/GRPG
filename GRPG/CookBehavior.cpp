#include "CookBehavior.h"
#include "entity.h"
#include "player.h"

string CookBehavior::displayText(){
	return "Cook " + food->getInventoryItem()->getItem()->getName();
}

void CookBehavior::action()
{

}