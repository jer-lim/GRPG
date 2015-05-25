#include "TakeArtifactBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "ArtifactDisplay.h"
#include "grpg.h"

string TakeArtifactBehavior::displayText(){
	return "Attempt to take the artifact";
}

void TakeArtifactBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (!qd->getValue("artifactDisplayStatus") == 1)
		{
			ui->addChatText("The display case is locked. Maybe something will unlock it?");
		}
		else
		{
			if (qd->getValue("artifactStealStatus") < 3)
			{
				ui->addChatText("The display case is unlocked. You take the artifact!");
				grpg->getGameEventManager()->informListeners(new GameEvent_ItemReceived(grpg->getItemLoader()->getItem(37)));

				((ArtifactDisplay*)entity)->setFrame(2);
			}
			else
			{
				ui->addChatText("The display case is empty.");
			}
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}