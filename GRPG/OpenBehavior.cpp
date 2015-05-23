#include "OpenBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "QuestData.h"
#include "RichDoor.h"

string OpenBehavior::displayText(){
	return "Attempt to unlock the door";
}

void OpenBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (!qd->getValue("artifactStealKey") == 1)
		{
			ui->addChatText("The door is locked, and you don't have the key to open it.");
		}
		else
		{
			ui->addChatText("You use the key to open the door");
			((RichDoor*)entity)->setOpen(true);
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}