#include "SaveCrystalUseBehavior.h"
#include "NPC.h"
#include "UI.h"
#include "SaveCrystal.h"
#include "player.h"
#include <fstream>
#include <iostream>
#include <string>
#include "grpg.h"

string SaveCrystalUseBehavior::displayText(){
	return "Use Save Crystal";
}

void SaveCrystalUseBehavior::action()
{
	VECTOR2 collisionVector;
	if (player->collidesWith(*entity, collisionVector) && !player->hasFailedThieve())
	{
		ui->drawWindow("Save Crystal");
		ui->addTalkText(new ChatInformation("This is a save crystal. It will allow you to save and load your games, as well as check your stats.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("What would you like to do?", chatNS::MIDDLE));
		ChatDecision* dt = new ChatDecision(chatNS::VERTICALLY);
		dt->setCaller(this);
		dt->addOption(3, "Save game");
		dt->addOption(4, "Load game");
		dt->addOption(5, "Check stats");
		dt->addOption(6, "More information...");
		dt->addOption(0, "Do nothing");
		ui->addTalkText(dt);
	}
	else
	{
		player->setVictim(entity);
		player->releaseDestination();
		player->setNPCAction(this);
	}
}

void SaveCrystalUseBehavior::optionSelected(ChatOption co)
{
	//The messages here are less of talking and more of selecting options, no need to reprint them.
	//ui->addTalkText(new ChatInformation(co.text, chatNS::LEFT));

	//Note to developers: This method will already initalize the final ChatDecision* here for you. Use it.
	//Caller has been set with a default displayType of VERTICALLY, call the set codes to change it if required
	//If not used, please call delete cd; to ensure that there are no leaks.
	ChatDecision* cd = new ChatDecision(chatNS::VERTICALLY);
	cd->setCaller(this);
	switch (co.id)
	{
	case 0: //Always exit
		ui->removeWindow();
		//No use with cd
		delete cd;
		break;
	case 1: //Yes to loading
	{
		bool result = ((SaveCrystal*)entity)->load();
		if (result)
		{
			ui->addTalkText(new ChatInformation("Game data loaded.", chatNS::MIDDLE));
		}
		else
		{
			ui->addTalkText(new ChatInformation("There was an error loading the save file. Your save file is likely corrupted.", chatNS::MIDDLE));
		}
		cd->addOption(5, "Check stats");
		cd->addOption(6, "More information...");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	}
	case 2: //No to loading
		ui->addTalkText(new ChatInformation("Alright, nothing's done.", chatNS::MIDDLE));
		cd->addOption(3, "Save game");
		cd->addOption(5, "Check stats");
		cd->addOption(6, "More information...");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 3:
		((SaveCrystal*)entity)->save();
		ui->addTalkText(new ChatInformation("Game data saved.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("You can save your game quickly by right clicking on the save crystal and choosing quicksave.", chatNS::MIDDLE));
		cd->addOption(5, "Check stats");
		cd->addOption(6, "More information...");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 4:
		ui->addTalkText(new ChatInformation("Are you sure? You will lose all unsaved data. All items on the ground will be removed.", chatNS::MIDDLE));
		cd->setDisplayType(chatNS::HORIZONTALLY);
		cd->addOption(1, "Yes");
		cd->addOption(2, "No");
		ui->addTalkText(cd);
		break;
	case 5:
	{
		stringstream ss;
		ss << "You have died " << player->getTotalDeaths() << " times";
		ui->addTalkText(new ChatInformation(ss.str(), chatNS::MIDDLE));
		ss.str("");
		ss << "You have killed Doombreather Aildiuln " << player->getAidilKills() << " time(s).";
		if (player->getAidilKillTime() != 0)
		{
			ss << " Your best time taken to kill him is " << player->getAidilKillTime() << " seconds.";
		}
		ui->addTalkText(new ChatInformation(ss.str(), chatNS::MIDDLE));
		cd->addOption(3, "Save game");
		cd->addOption(4, "Load game");
		cd->addOption(6, "More information...");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	}
	case 6:
		ui->addTalkText(new ChatInformation("The save crystal will allow you to save and load your game. There is only 1 save file available. You can also view your player stats here. (such as deaths, boss kills etc.)", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("Only certain items are saved and loaded.", chatNS::MIDDLE));
		cd->addOption(7, "Check what is saved");
		cd->addOption(8, "Check what happens on load");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 7:
		ui->addTalkText(new ChatInformation("The following items are saved: your skills, inventory items, quests, player stats. Everything else is not saved.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("Specifically, this means the following is not saved: your current position, all dropped items, any enemy health.", chatNS::MIDDLE));
		cd->addOption(8, "Check what happens on load");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	case 8:
		ui->addTalkText(new ChatInformation("On load, the entire game is reset to a clean slate, then your save data is loaded.", chatNS::MIDDLE));
		ui->addTalkText(new ChatInformation("To prevent exploits, this means that all items on the ground is deleted.", chatNS::MIDDLE));
		cd->addOption(7, "Check what is saved");
		cd->addOption(0, "Leave");
		ui->addTalkText(cd);
		break;
	}
}