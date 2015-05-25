#include "PullArtifactLever.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "grpg.h"
#include "ArtifactLever.h"

string PullArtifactLeverBehavior::displayText(){
	return "Pull the lever";
}

void PullArtifactLeverBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		((ArtifactLever*)entity)->setPulled(!((ArtifactLever*)entity)->getPulled());
		grpg->getGameEventManager()->informListeners(new GameEvent_EntityAction(entity->getPerson()));
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}