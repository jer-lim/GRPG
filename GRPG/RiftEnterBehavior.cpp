#include "RiftEnterBehavior.h"
#include "grpg.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "GameEvent_EntityAction.h"
#include "PersonLoader.h"
#include "ShriveledMan.h"
#include "Rift.h"
#include "mapLoader.h"

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
			player->setRiftPortal((Rift*)entity);
			//First time entry, intro in Mysterious Artifact
			if (grpg->getQuestLoader()->getQuestData()->getValue("mysteriousArtifactStatus") == 5)
			{
				//Move Shriveled man to north of here, start intro
				//Get reference to ShriveledMan
				VECTOR2 location = grpg->getMapLoader()->translateIdToCoords('?');
				stringstream ss;
				ss << location.x << ","  << location.y;
				ShriveledMan* sm = (ShriveledMan*)grpg->getSpawnLink(ss.str());
				sm->setY(entity->getY()-GAME_HEIGHT/4);
				sm->setX(entity->getX());
				sm->startRiftIntro();
			}
			else
			{
				((Rift*)entity)->begin();
			}
			((Rift*)entity)->setupBehaviors();
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}