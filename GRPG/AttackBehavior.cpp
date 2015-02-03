#include "AttackBehavior.h"
#include "NPC.h"
#include "player.h"

string AttackBehavior::displayText(){ 
	return "Attack " + npc->getname();
}

//player attacks enemy, and not anything else (is NOT a replacement for entity::attack)
void AttackBehavior::action()
{
	player->setVictim(entity);
	player->releaseDestination();
	player->setDestination(0);
}