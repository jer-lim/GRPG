#include "BlockRockMineBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "BlockRock.h"
#include "player.h"

string BlockRockMineBehavior::displayText(){
	return "Attempt to mine the rock";
}

void BlockRockMineBehavior::action()
{
	if (player->getY() > entity->getY())
	{
		ui->addChatText("The rock is too hard to mine it from this side.");
		ui->addChatText("Maybe try mining it from a different side?");
	}
	else
	{
		ui->addChatText("You attempt to mine the rock...");
		ui->addChatText("and successfully break through.");
		BlockRock* br = ((BlockRock*)entity);
		br->setDisabled(true);
		if (br->hasMiningBonus())
		{
			ui->addChatText("Mining this rock for the first time taught you more about mining.");
			player->getSkills()->at(skillNS::ID_SKILL_MINING).gainXP(1500);
			br->mined();
		}
		else
		{
			player->getSkills()->at(skillNS::ID_SKILL_MINING).gainXP(100);
		}
	}
}