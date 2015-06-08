#include "PlantPatchInspectBehavior.h"
#include "PlantPatch.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"

string PlantPatchInspectBehavior::displayText(){
	return "Inspect Plant Patch";
}

void PlantPatchInspectBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		ui->addChatText("This is a plant patch, for planting trees and flowers.");
		switch(((PlantPatch*)entity)->getStatus())
		{
		case plantPatchNS::WEEDS:
			ui->addChatText("It is currently filled with weeds and unsuitable for planting.");
			break;
		case plantPatchNS::EMPTY:
			ui->addChatText("It is currently empty.");
			break;
		case plantPatchNS::HOLE:
			ui->addChatText("There's a big hole in the middle of it, suitable for placing seeds or transfering young plants.");
			break;
		case plantPatchNS::PLANTED:
			ui->addChatText("A plant has already been planted in it.");
			break;
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}