//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#include "AggressivePluckBehavior.h"
#include "player.h"
#include "npc.h"
#include "UI.h"
#include "GameEvent_EntityAction.h"

string AggressivePluckBehavior::displayText(){
	return "Aggressively attempt to pluck feathers";
}

void AggressivePluckBehavior::action()
{
	VECTOR2 collisionVector;
	if (theChicken->collidesWith(*player, collisionVector))
	{
		//attempt to pluck feathers based off thieving level
		//This is on the Easter Event 2015 "Quest"
		int playerStrengthLevel = player->getSkills()->at(skillNS::ID_SKILL_STRENGTH).getSkillLevel();
		if (playerStrengthLevel < 4)
		{
			ui->addChatText("You need at a strength level of at least 4 to attempt that.");
		}
		else
		{
			ui->addChatText("You attempt to hold the chicken still and pluck some feathers...");
			bool success = rand() % 4 != 0;
			if (success)
			{
				ui->addChatText("You successfully plucked some feathers!");
				gem->informListeners(new GameEvent_EntityAction(npc));
			}
			else
			{
				ui->addChatText("Your strength fails at a crucial time and it quickly escapes!");
			}
		}
	}
	else
	{
		player->setVictim(theChicken);
		player->setNPCAction(this);
	}
}