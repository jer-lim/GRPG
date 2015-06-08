#include "PlantPatchPlantBehavior.h"
#include "PlantPatch.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string PlantPatchPlantBehavior::displayText(){
	if (questData->getValue("mysteriousArtifactGardenerTask") == 1)
	{
		return "Plant the flwoers in the Plant Patch";
	}
	else if (questData->getValue("mysteriousArtifactGardenerTask") == 2)
	{
		return "Transfer the tree to the Plant Patch";
	}
	else
	{
		return "Plant some seeds in the Plant Patch";
	}
}

void PlantPatchPlantBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (questData->getValue("mysteriousArtifactStatus") != 1) //Have not reached part of quest to do this.
		{
			ui->addChatText("I don't think the gardener would want me messing with this.");
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::HOLE)
		{
			if (questData->getValue("mysteriousArtifactGardenerTask") == 1)
			{
				ui->addChatText("You plant the flower seeds into the patch.");
				((PlantPatch*)entity)->setStatus(plantPatchNS::PLANTED);
			}
			else if (questData->getValue("mysteriousArtifactGardenerTask") == 2)
			{
				ui->addChatText("You need to have a young tree in your inventory first.");
			}
			else
			{
				ui->addChatText("You'll need some seeds first.");
			}
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::WEEDS)
		{
			ui->addChatText("You need to clear the weeds from that patch first!");
		}
		else if (((PlantPatch*)entity)->getStatus() == plantPatchNS::EMPTY)
		{
			ui->addChatText("You need to dig a hole first.");
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