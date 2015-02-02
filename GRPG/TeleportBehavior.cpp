#include "TeleportBehavior.h"
#include "player.h"
#include "NPC.h"

string TeleportBehavior::displayText(){
	return "Request teleport from " + npc->getname();
}

void TeleportBehavior::action()
{
	player->setX(coordinates.x);
	player->setY(coordinates.y);
}