#include "KeyFishBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "player.h"
#include "GameEventManager.h"
#include "GameEvent_ItemReceived.h"
#include "grpg.h"

string KeyFishBehavior::displayText(){
	return "Fish";
}

void KeyFishBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		if (grpg->getQuestLoader()->getQuestData()->getValue("artifactStealStatus") == 1)
		{
			if (player->getSkills()->at(skillNS::ID_SKILL_FISHING).getSkillLevel() < 5)
			{
				ui->addChatText("You need a fishing level of at least 5 to attempt to fish here.");
			}
			else
			{
				if (grpg->getQuestLoader()->getQuestData()->getValue("artifactStealKey") == 0)
				{
					ui->addChatText("You start fishing...");
					ui->addChatText("What's this? You maange to fish up a key?");
					grpg->getGameEventManager()->informListeners(new GameEvent_ItemReceived(grpg->getItemLoader()->getItem(36)));
				}
				else
				{
					ui->addChatText("You already have a key to the house.");
				}
			}
		}
		else
		{
			ui->addChatText("There doesn't appear to be any actual fish swimming here.");
			//ui->addChatText("You can't see any reason why you would want to do that...");
		}
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}