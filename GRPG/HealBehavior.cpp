//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "HealBehavior.h"
#include "player.h"
#include "npc.h"

string HealBehavior::displayText(){
	return "Request healing from " + npc->getname();
}

void HealBehavior::action()
{
	VECTOR2 collisionVector;
	if (guyDoingHealing->collidesWith(*player, collisionVector) && !player->hasFailedThieve())
	{
		player->setHealth(player->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel());
	}
	else
	{
		player->setVictim(guyDoingHealing);
		player->setNPCAction(this);
	}
}