//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "QuickPluckBehavior.h"
#include "player.h"
#include "npc.h"
#include "UI.h"
#include "GameEvent_EntityAction.h"

string QuickPluckBehavior::displayText(){
	return "Stealthily attempt to pluck feathers";
}

void QuickPluckBehavior::action()
{
	VECTOR2 collisionVector;
	if (theChicken->collidesWith(*player, collisionVector))
	{
		//attempt to pluck feathers based off thieving level
		//This is on the Easter Event 2015 "Quest"
		int playerStealLevel = player->getSkills()->at(skillNS::ID_SKILL_THIEVING).getSkillLevel();
		if (playerStealLevel < 7)
		{
			ui->addChatText("You need at a thieving level of at least 7 to attempt that.");
		}
		else
		{
			ui->addChatText("You attempt to stealthily pluck some feathers...");
			bool success = true;
			int random = rand() % playerStealLevel;
			if (random <= 7)
			{
				if (rand() % 2 == 0)
				{
					success = false;
				}
			}
			if (success)
			{
				ui->addChatText("You successfully plucked some feathers!");
				gem->informListeners(new GameEvent_EntityAction(npc));
			}
			else
			{
				ui->addChatText("The chicken's senses detect you and it quickly escapes!");
			}
		}
	}
	else
	{
		player->setVictim(theChicken);
		player->setNPCAction(this);
	}
}