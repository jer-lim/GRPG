#include "PickLockBehavior.h"
#include "RichDoor.h"
#include "NPC.h"
#include "UI.h"
#include "BlockRock.h"
#include "player.h"

string PickLockBehavior::displayText(){
	return "Attempt to pick the lock";
}

void PickLockBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (player->getSkills()->at(skillNS::ID_SKILL_THIEVING).getSkillLevel() < 10)
		{
			ui->addChatText("You need a thieving level of at least 10 to attempt to pick the lock");
		}
		else
		{
			ui->addChatText("You attempt to pick the lock...");
			ui->addChatText("You manage to successfully pick the lock!");
			questData->setValue("artifactStealMethod", 2);
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