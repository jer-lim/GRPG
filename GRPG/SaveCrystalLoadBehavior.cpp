#include "SaveCrystalLoadBehavior.h"
#include "player.h"
#include "npc.h"
#include "SaveCrystal.h"
#include "UI.h"

string SaveCrystalLoadBehavior::displayText(){
	return "Load recent save with Save Crystal";
}

void SaveCrystalLoadBehavior::action()
{
	VECTOR2 collisionVector;
	if (entity->collidesWith(*player, collisionVector))
	{
		((SaveCrystal*)entity)->load();
	}
	else
	{
		player->setVictim(entity);
		player->setNPCAction(this);
	}
}