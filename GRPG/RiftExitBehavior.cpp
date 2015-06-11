#include "RiftExitBehavior.h"
#include "grpg.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "GameEvent_EntityAction.h"
#include "GameEvent_RiftClosed.h"
#include "PersonLoader.h"
#include "Rift.h"

string RiftExitBehavior::displayText(){
	switch (((Rift*)entity)->getWaveStatus())
	{
	case riftNS::STARTED:
	case riftNS::STARTING:
	case riftNS::COMPLETED:
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
		//Check if this is during the mysterious artifact quest
		if (grpg->getQuestLoader()->getQuestData()->getValue("mysteriousArtifactStatus") == 5)
		{
			//It is; don't do anything, the player needs to enter in here to complete the quest.
			((Rift*)entity)->setupBehaviors();
		}
		else
		{
			grpg->getGameEventManager()->informListeners(new GameEvent_RiftClosed((Rift*)entity));
			if (grpg->getQuestLoader()->getQuestData()->getValue("mysteriousArtifactStatus") == 7)
			{
				//Do some special stuff for the mysterious artifact quest
				//Spawn the owner of the artifact nearby
				NPC::spawn(grpg, 40, entity->getVector());
			}
			((Rift*)entity)->preMatureExit();
			((Rift*)entity)->awardXP();
			((Rift*)entity)->close();
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}