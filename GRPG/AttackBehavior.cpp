#include "AttackBehavior.h"
#include "NPC.h"

string AttackBehavior::displayText(){ 
	return "Attack " + npc->getname();
}
void AttackBehavior::action(){}//player attacks enemy, and not anything else (is NOT a replacement for entity::attack)