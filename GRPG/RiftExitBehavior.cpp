#include "RiftExitBehavior.h"
#include "grpg.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "GameEvent_EntityAction.h"
#include "PersonLoader.h"
#include "Rift.h"

string RiftExitBehavior::displayText(){
	switch (((Rift*)entity)->getWaveStatus())
	{
	case riftNS::STARTED:
	case riftNS::STARTING:
		return "Safely Exit Rift.";
	case riftNS::PROGRESSING:
		return "Recklessly Exit Rift";
	default:
		return "Exit Rift";
	}
}

void RiftExitBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		player->setRiftPortal(nullptr);
		((Rift*)entity)->setupBehaviors();
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}