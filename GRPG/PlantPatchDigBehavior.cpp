#include "PlantPatchDigBehavior.h"
#include "PlantPatch.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string PlantPatchDigBehavior::displayText(){
	return "Dig a hole in the Plant Patch";
}

void PlantPatchDigBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (questData->getValue("mysteriousArtifactStatus") == 0) //Have not reached part of quest to do this.
		{
			ui->addChatText("I don't think the gardener would want me messing with this.");
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::EMPTY)
		{
			ui->addChatText("You use a spade to dig a hole in the patch.");
			ui->addChatText("Great, now there's a large ugly hole in it.");
			((PlantPatch*)entity)->setStatus(plantPatchNS::HOLE);
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::WEEDS)
		{
			ui->addChatText("You need to clear the weeds from that patch first!");
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::HOLE)
		{
			ui->addChatText("There's already a huge hole in the patch!");
		}
		else
		{
			ui->addChatText("There's no reason to do that at the moment.");
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}