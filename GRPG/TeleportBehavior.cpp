#include "TeleportBehavior.h"
#include "player.h"
#include "NPC.h"
#include "mapLoader.h"

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