#include "SaveCrystalQuickSaveBehavior.h"
#include "player.h"
#include "npc.h"
#include "SaveCrystal.h"
#include "UI.h"

string SaveCrystalQuickSaveBehavior::displayText(){
	return "QuickSave with Save Crystal";
}

void SaveCrystalQuickSaveBehavior::action()
{
	VECTOR2 collisionVector;
	if (entity->collidesWith(*player, collisionVector))
	{
		((SaveCrystal*)entity)->save();
	}
	else
	{
		player->setVictim(entity);
		player->setNPCAction(this);
	}
}