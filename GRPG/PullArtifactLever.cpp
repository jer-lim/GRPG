#include "PullArtifactLever.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "grpg.h"
#include "ArtifactLever.h"
#include "PersonLoader.h"

string PullArtifactLeverBehavior::displayText(){
	return "Pull the lever";
}

void PullArtifactLeverBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		((ArtifactLever*)entity)->setPulled(!((ArtifactLever*)entity)->getPulled());
		grpg->getGameEventManager()->informListeners(new GameEvent_EntityAction(grpg->getPersonLoader()->getNPC(36)));
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}