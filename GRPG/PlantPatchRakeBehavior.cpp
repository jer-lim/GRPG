#include "PlantPatchRakeBehavior.h"
#include "PlantPatch.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string PlantPatchRakeBehavior::displayText(){
	return "Rake Plant Patch";
}

void PlantPatchRakeBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (questData->getValue("mysteriousArtifactStatus") != 1) //Have not reached part of quest to do this.
		{
			ui->addChatText("I don't think the gardener would want me messing with this.");
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::WEEDS)
		{
			ui->addChatText("You rake the patch, removing the weeds from it.");
			((PlantPatch*)entity)->setStatus(plantPatchNS::EMPTY);
		}
		else
		{
			ui->addChatText("The plant patch does not currently need weeding.");
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}