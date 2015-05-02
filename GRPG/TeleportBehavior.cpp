//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "TeleportBehavior.h"
#include "player.h"
#include "NPC.h"
#include "mapLoader.h"
#include "grpg.h"

string TeleportBehavior::displayText(){
	if (guyDoingTeleport->getPerson() != nullptr)
	{
		if (guyDoingTeleport->getPerson()->getType() == "NPC")
			return "Request teleport from " + ((NPC*)guyDoingTeleport->getPerson())->getname();
	}
	else if (guyDoingTeleport->getInventoryItem() != nullptr)
	{
		return "Use " + guyDoingTeleport->getInventoryItem()->getItem()->getName() + " to teleport";
	}
}

void TeleportBehavior::action()
{
	if (guyDoingTeleport->getInventoryItem() != nullptr && !guyDoingTeleport->getAnchored()) //Not anchored = not in inventory
	{
		((Grpg*)gameptr)->getUI()->addChatText("You need to pick that up first.");
		return;
	}
	VECTOR2 collisionVector;
	if (guyDoingTeleport->collidesWith(*player, collisionVector) || guyDoingTeleport->getInventoryItem() != nullptr)
	{
		VECTOR2 coordinates = ml->translateIdToCoords(tileID);
		player->setX(coordinates.x);
		player->setY(coordinates.y);
		player->setVictim(0);
		player->releaseDestination();
		gameptr->setMouseOverEntity(nullptr);
	}
	else
	{
		player->releaseDestination();
		player->setVictim(guyDoingTeleport);
		player->setNPCAction(this);
	}
}