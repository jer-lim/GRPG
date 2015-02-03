#include "HealBehavior.h"
#include "player.h"
#include "npc.h"

string HealBehavior::displayText(){
	return "Request healing from " + npc->getname();
}

void HealBehavior::action()
{
	VECTOR2 collisionVector;
	if (guyDoingHealing->collidesWith(*player, collisionVector))
	{
		player->setHealth(player->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel());
	}
	else
	{
		player->setVictim(guyDoingHealing);
		player->setNPCAction(this);
	}
}