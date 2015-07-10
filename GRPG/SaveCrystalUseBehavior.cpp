#include "SaveCrystalUseBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "SaveCrystal.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include <string>

string SaveCrystalUseBehavior::displayText(){
	return "Use Save Crystal";
}

void SaveCrystalUseBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		ui->addChatText("Save data saved.");
		//ui->drawWindow("Save Crystal");
		//ui->addTalkText(new ChatInformation("This "))
		ofstream out("savefile.txt", ios::binary);
		out.write(player->getSkills()->at(1).getSkill()->getName().c_str(), sizeof(player->getSkills()->at(1).getSkill()->getName().c_str()));
		out.close();
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}