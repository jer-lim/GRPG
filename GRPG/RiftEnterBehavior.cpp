#include "RiftEnterBehavior.h"
#include "grpg.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "GameEvent_EntityAction.h"
#include "PersonLoader.h"

string RiftEnterBehavior::displayText(){
	return "Enter Rift";
}

void RiftEnterBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (grpg->getQuestLoader()->getQuestData()->getValue("mysteriousArtifactStatus") == 3)
		{
			ui->addChatText("I should talk to Alfred before jumping into this rift.");
		}
		else
		{
			grpg->getGameEventManager()->informListeners(new GameEvent_EntityAction(grpg->getPersonLoader()->getNPC(39)));
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}