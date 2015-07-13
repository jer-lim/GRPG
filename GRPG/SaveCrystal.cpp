#include "SaveCrystal.h"
#include "SaveCrystalUseBehavior.h"
#include "SaveCrystalQuickSaveBehavior.h"
#include "Aidil.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"

#include <cereal\types\map.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\types\complex.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal\cereal.hpp>

#include <iostream>
#include <fstream>

SaveCrystal::SaveCrystal() : Entity()
{
	saveCrystalTexture = new TextureManager();
}

SaveCrystal::~SaveCrystal()
{
	SAFE_DELETE(saveCrystalTexture);
	SAFE_DELETE(quickSaveBehavior);
	SAFE_DELETE(useBehavior);
}

bool SaveCrystal::initialize(Game* gamePtr, Player* p, Destination* location, string et)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	examineText = et;

	if (!saveCrystalTexture->initialize(graphics, saveCrystalNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize save crystal texture.");
	}
	bool result = Entity::initialize(gamePtr, saveCrystalNS::imageWidth, saveCrystalNS::imageHeight, saveCrystalNS::frames, saveCrystalTexture);
	image.setFrameDelay(0.3f);
	image.setFrames(0, saveCrystalNS::frames - 1);
	setupBehaviors();
	setX(location->getX());
	setY(location->getY());

	//Don't display me if this is the first time the player is running the game
	//and I'm the save crystal at the tutorial zome
	if (!saveFileExists())
	{
		VECTOR2 startSaveCrystalCoords = ((Grpg*)theGame)->getMapLoader()->translateIdToCoords('.');
		if (x == startSaveCrystalCoords.x && y == startSaveCrystalCoords.y)
		{
			fakeDelete();
		}
	}

	return result;
}

void SaveCrystal::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void SaveCrystal::update(float frameTime, Game* gamePtr)
{
	//Rock never moves, attacks ... or get attacked, ... or do anything, really.
	//Entity::update(frameTime, gamePtr);
	image.update(frameTime);

	//Check for collision with player
	/*
	VECTOR2 collisionVector;
	if (thePlayer->collidesWith(*this, collisionVector))
	{
		//Nope, no crossing.
		if (thePlayer->getY() > getY())
		{
			thePlayer->setY(getY() + blockRockNS::imageHeight / 2 + blockRockNS::collideMargin);
		}
		else
		{
			thePlayer->setY(getY() - blockRockNS::imageHeight / 2 - blockRockNS::collideMargin);
		}
		thePlayer->releaseDestination();
		thePlayer->setVictim(nullptr);
		ui->addChatText("You can't seem to find a way around this rock");
	}*/
}

void SaveCrystal::save()
{
	map<string, map<string, int>> saveData;
	ui->addChatText("Game saved.");
	//ui->drawWindow("Save Crystal");
	//ui->addTalkText(new ChatInformation("This "))
	saveData["questData"] = ((Grpg*)theGame)->getQuestLoader()->getQuestData()->getAllValues();
	saveData["skillsData"] = thePlayer->getSkillsToSave();
	map<string, int> otherData;
	otherData[thePlayer->getInventory()->getInventoryString()] = 0;
	saveData["inventory"] = otherData;
	map<string, int> questData;
	map<int, Quest*>* questList = ((Grpg*)theGame)->getQuestLoader()->getMapQuests();
	for (map<int, Quest*>::iterator i = questList->begin(); i != questList->end(); ++i)
	{
		questData[i->second->getQuestString()] = i->first;
	}
	saveData["quests"] = questData;
	map<string, int> playerData;
	playerData["deaths"] = thePlayer->getTotalDeaths();
	saveData["player"] = playerData;

	ofstream myfile;
	myfile.open("savefile.gdef");
	cereal::JSONOutputArchive output(myfile);
	output(cereal::make_nvp("savedata", saveData));
}

bool SaveCrystal::load()
{
	Grpg* grpg = (Grpg*)theGame;

	grpg->getDrawManager()->removeAllDroppedItems();

	map<string, map<string, int>> loadData;
	ifstream loadFile("savefile.gdef");
	if (!loadFile.good())
	{
		ui->addChatText("No save files found");
		return false;
	}
	if (loadFile.is_open())
	{
		cereal::JSONInputArchive i_archive(loadFile);
		i_archive(loadData);
	}
	thePlayer->loadSkills(loadData["skillsData"]);
	grpg->getQuestLoader()->getQuestData()->loadQuestData(loadData["questData"]);
	vector<string> keys;
	for (map<string, int>::iterator i = loadData["inventory"].begin(); i != loadData["inventory"].end(); i++)
	{
		keys.push_back(i->first);
	}
	thePlayer->getInventory()->loadInventoryString(keys[0], grpg->getItemLoader(), grpg);
	//Delete and reload all quests, they may have progressed passed their save state
	map<int, Quest*>* mapQuests = grpg->getQuestLoader()->getMapQuests();
	for (map<int, Quest*>::iterator i = mapQuests->begin(); i != mapQuests->end(); ++i)
	{
		//Remove them
		grpg->getGameEventManager()->removeListener(i->second);
		SAFE_DELETE(i->second);
	}
	mapQuests->clear();

	grpg->getQuestLoader()->loadAllQuests(grpg->getGameEventManager(), grpg->getPersonLoader(), grpg->getItemLoader(), grpg->getGraphics(), ui->getTopLeftX(), ui->getTopLeftY());
	mapQuests = grpg->getQuestLoader()->getMapQuests();
	for (map<string, int>::iterator i = loadData["quests"].begin(); i != loadData["quests"].end(); i++)
	{
		mapQuests->at(i->second)->loadQuestString(i->first);
	}

	map<string, int> playerData = loadData["player"];
	thePlayer->setTotalDeaths(playerData["deaths"]);

	ui->addChatText("Game loaded.");
	//Move player to the save crystal near the doctor
	VECTOR2 coords = grpg->getMapLoader()->translateIdToCoords(',');
	thePlayer->setX(coords.x);
	thePlayer->setY(coords.y);
	thePlayer->setVictim(0);
	thePlayer->releaseDestination();
	grpg->setMouseOverEntity(nullptr);
	thePlayer->setIsTeleporting(2);
	return true;
}

bool SaveCrystal::saveFileExists()
{
	map<string, map<string, int>> loadData;
	ifstream loadFile("savefile.gdef");
	return loadFile.good();
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void SaveCrystal::onLostDevice()
{
	saveCrystalTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void SaveCrystal::onResetDevice()
{
	saveCrystalTexture->onResetDevice();
}

void SaveCrystal::setupBehaviors()
{
	viewBehavior = new ViewBehavior("Save Crystal", examineText, ui);
	useBehavior = new SaveCrystalUseBehavior(thePlayer, this, ui, (Grpg*) theGame);
	quickSaveBehavior = new SaveCrystalQuickSaveBehavior(thePlayer, this, ui);
	setupVectorActiveBehaviors();
}