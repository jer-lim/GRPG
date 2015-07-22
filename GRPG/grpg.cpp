//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Jeremy Lim
//  Student Number:     s10122326F
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "grpg.h"
#include "drawManager.h"
#include "mapLoader.h"
#include "tile.h"
#include "PersonLoader.h"
#include <sstream>
#include <random>
#include "ChatData.h"
#include "InventoryBoost.h"
#include "Aidil.h"
#include "NoiseManager.h"
#include "RiftData.h"

#include <cereal\types\map.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\types\complex.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal\cereal.hpp>

#include <iostream>
#include <fstream>

//=============================================================================
// Constructor
//=============================================================================
Grpg::Grpg()
{//Mem leak free
	dragonfireTexture = new TextureManager();
	uiFont = new TextDX();
	easterEggCounter = 0;
}

//=============================================================================
// Destructor
//=============================================================================
Grpg::~Grpg()
{
	//Make it stop playing music
	if (!SoundManager::getMuted())
	{
		SoundManager::toggleMute();
	}
    releaseAll();           // call onLostDevice() for every graphics item
	SAFE_DELETE(uiFont);
	//SAFE_DELETE(ui);//handled by drawmanager
	SAFE_DELETE(hitSplat);
	SAFE_DELETE(missSplat);
	SAFE_DELETE(viewport);
	Skill::deleteAllSkills();
	player->releaseDestination();
	SAFE_DELETE(drawManager);
	SAFE_DELETE(mapLoader);
	//Delete all the enemies that have been spawned
	/*//handled by drawmanager
	for (map<string, Entity*>::iterator it = spawnLinks.begin(); it != spawnLinks.end(); ++it)
	{
		if (it->second != nullptr)
		{
			SAFE_DELETE(it->second);
		}
	}*/
	SAFE_DELETE(personLoader);//to be called after all person entities are gone
	//SAFE_DELETE(player);//handled by drawmanager
	SAFE_DELETE(Person::thePlayer);
	SAFE_DELETE(itemLoader);//to be called after all item entities are gone
	SAFE_DELETE(gameEventManager);
	SAFE_DELETE(dragonfireTexture);
	SAFE_DELETE(questLoader);
	//~Game() called afterward
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Grpg::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	/* initialize random seed: */
	srand(time(NULL));

	Skill::setupAllSkills();

	mapLoader->loadData();

	// Set viewport
	viewport->setX(startLocation.x);
	viewport->setY(startLocation.y);

	// Load data
	itemLoader = new ItemLoader();
	itemLoader->loadAllItems();
	personLoader->loadAllNPCs(itemLoader);//ml free

	riftData = new RiftData();
	riftData->loadData();

	missSplat = new TextureManager();
	missSplat->initialize(graphics, MISS_IMAGE);
	entityNS::miss.initialize(graphics, 0, 0, 0, missSplat);

	hitSplat = new TextureManager();
	hitSplat->initialize(graphics, HIT_IMAGE);
	entityNS::hit.initialize(graphics, 0, 0, 0, hitSplat);

	if (!dragonfireTexture->initialize(graphics, aidilNS::dragonfireLocation))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize dragonfire texture.");
	}

	entityNS::splatText.initialize(graphics, 12, false, false, "Arial");
	entityNS::splatText.setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));
	entityNS::messageFont.initialize(graphics, 20, true, false, "Arial");
	entityNS::messageFont.setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));

	//Setup a generic YES/NO decision
	chatNS::YESNO = ChatDecision(chatNS::HORIZONTALLY);
	ChatOption cp, cp2;
	cp.id = 1;
	cp.text = "Yes";
	cp2.id = 2;
	cp2.text = "No";
	chatNS::YESNO.addOption(cp);
	chatNS::YESNO.addOption(cp2);

	// initialize DirectX fonts
	// 15 pixel high Arial
	if (uiFont->initialize(graphics, 20, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));
	//ml free
	// Initialise entities
	player = new Player();
	ui = new UI();

	if(!player->initialize(this))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the player"));

	if (!ui->initialize(this, player, input))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the user interface"));

	ui->setX(uiNS::X);
	ui->setY(uiNS::Y);
	player->setInventory(new Inventory());
	//Init position of inventory inside inventory to init positioning of inventory items upon their adding into them
	player->getInventory()->setXDrawPosition(ui->getTopLeftX());
	player->getInventory()->setYDrawPosition(ui->getTopLeftY());
	
	//Quest buttons need the getTopLeftX and Y
	questLoader = new QuestLoader();
	gameEventManager = new GameEventManager(ui, questLoader->getQuestData());
	questLoader->loadAllQuests(gameEventManager, personLoader, itemLoader, graphics, ui->getTopLeftX(), ui->getTopLeftY());

	drawManager->addObject(player,3);
	drawManager->addObject(ui, 999);
	// Load and display map, start spawners
	mapLoader->setVictim(player);
	mapLoader->loadMap();

	player->setX(startLocation.x);
	player->setY(startLocation.y);

	leftMouseWasDown = input->getMouseLButton();
	rightMouseWasDown = input->getMouseRButton();

	/*
	InventoryItem* y = new InventoryItem(itemLoader->getItem(0), 9);
	Entity* e = new Entity();
	e->initialize(this, y, true);//anchored if its an inventory
	//y->initialize(this, true);
	player->getInventory()->addEntityInventoryItem(e);
	
	//ml free
	//Object test
	//CRIME SCENE
	*//*
	InventoryItem* x = new InventoryItem(itemLoader->getItem(0), 9900);
	Entity* newObj = new Entity();
	newObj->initialize(this, x, false);
	//x->initialize(this, false);
	newObj->setX(startLocation.x);
	newObj->setY(startLocation.y);
	drawManager->addObject(newObj, 2);*/
	/*
	InventoryItem* f = new InventoryItem(itemLoader->getItem(7), 1);
	newObj = new Entity();
	newObj->initialize(this, f, false);
	//x->initialize(this, false);
	newObj->setX(startLocation.x);
	newObj->setY(startLocation.y);
	drawManager->addObject(newObj, 2);

	f = new InventoryItem(itemLoader->getItem(8), 1);
	newObj = new Entity();
	newObj->initialize(this, f, false);
	//x->initialize(this, false);
	newObj->setX(startLocation.x);
	newObj->setY(startLocation.y);
	drawManager->addObject(newObj, 2);
	*/
	//END SCENE

	//Play music
	SoundManager::initialize();
	SoundManager::playMusic(soundManagerNS::generalMusicID);
	analyticsDelay = GrpgNS::startingAnalyticsDelay;

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Grpg::update()
{
	if (!input->getMouseLButton() && leftMouseWasDown)
	{
		ui->setShowMainMenu(false);
		bool actionSuccess = false;
		bool inUI = false;
		if (ui->mouseInside(viewport))
		{
			actionSuccess = ui->performClick();
			inUI = true;
		}
		else
		{
			ui->removeWindow();
		}
		//else
		//{
		//If nothing happened in the ui
		if (!actionSuccess && mouseOverEntity != nullptr)
		{
			Behavior* topBehavior = mouseOverEntity->getTopMostBehavior();
			if (topBehavior != nullptr)
			{
				mouseOverEntity->getTopMostBehavior()->action();
				//mouseOverEntity = nullptr;
				actionSuccess = true;
			}
		}
		//If nothing happened and the player did not click in the ui
		if (!inUI && !actionSuccess)
		{
			VECTOR2 vpCoords = viewport->reverseTranslate(input->getMouseX(), input->getMouseY());

			Point* p = new Point(vpCoords.x, vpCoords.y);
			player->move(p);
			player->setVictim(0);
		}
		//}
		leftMouseWasDown = false;
		ui->removeRightClickMenu();
	}

	if (!input->getMouseRButton() && rightMouseWasDown)
	{
		//Show right click popup
		if(mouseOverEntity != nullptr)
		{
			//Find all the entities that the mouse is currently over
			map<int, map<int, ManagedObject*>> allEntities = drawManager->getDrawnObjects();
			for (map<int, map<int, ManagedObject*>>::reverse_iterator it = allEntities.rbegin(); it != allEntities.rend(); ++it){
				int zi = it->first;
				for (map<int, ManagedObject*>::reverse_iterator it2 = allEntities[zi].rbegin(); it2 != allEntities[zi].rend(); ++it2){
					if (it2->second->toBeErased)
					{
						//do nothing
					}
					else
					{
						if (it2->second->entity != nullptr && it2->second->entity->getPerson() != Person::thePlayer)
						{
							if (it2->second->entity->getType() == "UI")
							{//check inventory items
								if (((UI*)it2->second->entity)->getActiveTab() == uiNS::INVENTORY)
								{
									map<int, Entity*>* slotList = player->getInventory()->getSlotList();
									for (std::map<int, Entity*>::iterator it3 = slotList->begin(); it3 != slotList->end(); ++it3)
									{
										if (it3->second->mouseInside(viewport))
										{
											addMouseOverEntity(it3->second);
											break; //The player can only mouse over only 1 item in his inventory at a time
										}
									}
								}
								//Check shop items
								vector<Entity*> shopItems = ((UI*)it2->second->entity)->getShopItems();
								for (vector<Entity*>::iterator it3 = shopItems.begin(); it3 != shopItems.end(); ++it3)
								{
									Entity* theItem = *it3;
									if (theItem->mouseInside(viewport))
									{
										addMouseOverEntity(theItem);
										break;
									}
								}
							}
							else if (it2->second->entity->mouseInside(viewport))
							{
								//Do not use if realms are different
								if (player->inDarkRealm() == it2->second->entity->inDarkRealm())
								{
									addMouseOverEntity(it2->second->entity);
								}
							}
						}
					}
				}
			}

			//Build a list of vector<Behavior*> that is a combination o all the items
			//that the user is currently moused over
			vector<Behavior*> behaviors;
			for (std::vector<Entity*>::iterator it = mouseOverEntities.begin(); it != mouseOverEntities.end(); ++it) {
				Entity* item = *it;
				vector<Behavior*>behaviorsToAdd = item->getVectorActiveBehaviors();
				//Can't shortcut the above code and the below code, otherwise you get Vector Iterators Incompatible
				//https://stackoverflow.com/questions/8421623/vector-iterators-incompatible
				behaviors.insert(behaviors.end(), behaviorsToAdd.begin(), behaviorsToAdd.end());
			}
			ui->setRightClickMenu(behaviors);
			rightMouseWasDown = false;
			deleteAllMouseOverEntities();
		}
	}
	
	if (input->getMouseLButton())
	{
		leftMouseWasDown = true;
	}

	if (input->getMouseRButton())
	{
		rightMouseWasDown = true;
	}
	
	if (pro)
	{
		map<int, PlayerSkill>::iterator it;
		map<int, PlayerSkill>* playerSkills = player->getSkills();
		for (it = playerSkills->begin(); it != playerSkills->end(); it++)
		{
			it->second.gainXP(rand() % 10);
		}
	}
	mapLoader->update(frameTime);
	drawManager->updateAll(frameTime);
	SoundManager::update(frameTime, this);

	//player->setX(player->getX() + 2);
	
	//View is always centered on the player
	viewport->setX(player->getX());
	viewport->setY(player->getY());

	//Rift spawning checks
	if (questLoader->getQuestData()->getValue("mysteriousArtifactStatus") == 8)
	{
		stringstream ss;
		if (riftsInGame.size() != 0)
		{
			//Check for dead rifts
			for (int i = 0; i < riftsInGame.size(); i++)
			{
				ss << GrpgNS::riftSpawnPhrase << i;
				if (this->getSpawnLink(ss.str()) == nullptr)
				{
					riftsInGame[i] = nullptr;
				}
				ss.str("");
			}
		}
		else
		{
			for (int i = 0; i < GrpgNS::riftLimit; i++)
			{
				riftsInGame.push_back(nullptr);
			}
		}
		//We can spawn rifts now!
		//Search for dead rifts and spawn them at a random location
		VECTOR2 topLeftAllowedSpawn;
		VECTOR2 bottomRightAllowedSpawn;
		bool initalized = false;
		for (int i = 0; i < GrpgNS::riftLimit; i++)
		{
			if (riftsInGame[i] == nullptr)
			{
				if (!initalized)
				{
					//Cannot run these every frame, EXTREMELY EXPENSIVE
					topLeftAllowedSpawn = mapLoader->translateIdToCoords('{');
					bottomRightAllowedSpawn = mapLoader->translateIdToCoords('}');
					initalized = true;
				}
				//Pick a random location
				float randomX = rand() * 1.0 / RAND_MAX;
				randomX = randomX * (bottomRightAllowedSpawn.x - topLeftAllowedSpawn.x);
				randomX += topLeftAllowedSpawn.x;
				float randomY = rand() * 1.0 / RAND_MAX;
				randomY = randomY * (bottomRightAllowedSpawn.y - topLeftAllowedSpawn.y);
				randomY += topLeftAllowedSpawn.y;
				VECTOR2 newLocation = VECTOR2(randomX, randomY);
				riftsInGame[i] = (Rift*)NPC::spawn(this, 39, newLocation);
				ss << GrpgNS::riftSpawnPhrase << i;
				this->addSpawnLink(ss.str(), riftsInGame[i]);
			}
			ss.str("");
		}
	}
	else
	{
		//No rifts should spawn, check if any rifts are up and despawn them
		//This will occur if the user completed the quest but then loaded a save from before
		stringstream ss;
		if (riftsInGame.size() > 0)
		{
			//Check for dead rifts
			for (int i = 0; i < riftsInGame.size(); i++)
			{
				ss << GrpgNS::riftSpawnPhrase << i;
				if (this->getSpawnLink(ss.str()) == nullptr)
				{
					riftsInGame[i] = nullptr;
				}
				else
				{
					riftsInGame[i]->close();
				}
				ss.str("");
			}
			riftsInGame.clear();
		}
	}

	analyticsDelay -= frameTime;
	if (analyticsDelay <= 0)
	{
		saveAnalyticsData();
		analyticsDelay = GrpgNS::startingAnalyticsDelay;
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Grpg::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Grpg::collisions()
{
    VECTOR2 collisionVector;
}

//=============================================================================
// Render game items
//=============================================================================
void Grpg::render()
{
    graphics->spriteBegin();                // begin drawing sprites

	drawManager->renderAll();
	stringstream ss;
	//ss << player->getSkills()->at(skillNS::ID_SKILL_ATTACK).getXP();
	ss << "FPS: " << fps;
	uiFont->print(ss.str(), 5, 0); //Feel free to use this text for any debugging thing
	ss.str("");
	//Handle mouseover
	if (mouseOverEntity != nullptr)
	{
		//do a proper interface check here for the name
		Behavior* topMostBehavior = mouseOverEntity->getTopMostBehavior();
		if (topMostBehavior != nullptr)
		{
			uiFont->print(topMostBehavior->displayText(), 5, 20);
		}
	}

    graphics->spriteEnd();                  // end drawing sprites
	
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Grpg::releaseAll()
{
	drawManager->releaseAll();
	uiFont->onLostDevice();
	//UI is their own class as well, and needs to be told to release their inner children's
	//textures and text (The entity manager only does it for the texture)
	ui->onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Grpg::resetAll()
{
	drawManager->resetAll();
	uiFont->onResetDevice();
	//UI is their own class as well, and needs to be told to release their inner children's
	//textures and text (The entity manager only does it for the texture)
	ui->onResetDevice();

    Game::resetAll();
    return;
}


//=============================================================================
// Processes the command entered, performing different actions depending on the command
// Returns true if command was processed, false on no processing
//=============================================================================
bool Grpg::processCommand(std::string command)
{
	if (command == "pro")
	{
		pro = !pro;
		return true;
	}
	else if (command == "easter")
	{
		//Send event that player just talked to the Easter Bunny
		GameEvent* ge = new GameEvent_EntityAction(personLoader->getNPC(28));
		gameEventManager->informListeners(ge);
		return true;
	}
	else if (command == "shop")
	{
		ui->drawWindow("Solomon's General Store");
		ui->setShopItems(vector<Entity*>());
		return true;
	}
	else if (command == "more")
	{
		drawManager->addObject(player, 3);
		return true;
	}
	else if (command == "fish")
	{
		player->startFishing(false);
		return true;
	}
	else if (command == "mine")
	{
		player->startMining(false);
		return true;
	}
	else if (command.substr(0, 2) == "tp")
	{
		std::string location = command.substr(3);
		VECTOR2 coordinates;
		if (location == "shop")
			coordinates = mapLoader->translateIdToCoords('$');
		else if (location == "doctor")
			coordinates = mapLoader->translateIdToCoords('+');
		else if (location == "stove")
			coordinates = mapLoader->translateIdToCoords('b'); 
		else
			coordinates = mapLoader->translateIdToCoords(*(location.c_str()));

		player->setX(coordinates.x);
		player->setY(coordinates.y);
		player->setVictim(0);
		player->releaseDestination();
		mouseOverEntity = nullptr;
		player->setIsTeleporting(2);

		return true;
	}
	else if (command.substr(0, 5) == "spawn")
	{

		std::string enemyToSpawn = command.substr(6);
		int enemyId = stoi(enemyToSpawn);

		NPC::spawn(this, enemyId, VECTOR2(player->getX() + 50, player->getY() - 50), player);
		return true;
	}
	else if (command.substr(0, 2) == "sk")
	{
		int a = stoi(command.substr(3));
		stringstream ss = stringstream();
		ss << Skill::calculateXPRequired(a);
		ui->addChatText(ss.str());
		return true;
	}
	else if (command.substr(0, 2) == "se")
	{
		bool error = command.length() < 6;
		if (!error)
		{
			std::string eventType = command.substr(3, 2);
			std::string furtherSubtype = command.substr(6);
			if (eventType == "ea")
			{
				int npcId = stoi(furtherSubtype);
				gameEventManager->informListeners(new GameEvent_EntityAction(personLoader->getNPC(npcId)));
				ui->addChatText("GameEvent_EntityAction sent with Entity: " + personLoader->getNPC(npcId)->getname());
			}
			else if (eventType == "ir")
			{
				int itemId = stoi(furtherSubtype);
				gameEventManager->informListeners(new GameEvent_ItemReceived(itemLoader->getItem(itemId)));
				ui->addChatText("GameEvent_ItemReceived sent with Item: " + itemLoader->getItem(itemId)->getName());
			}
			else if (eventType == "iu")
			{
				int itemId = stoi(furtherSubtype);
				gameEventManager->informListeners(new GameEvent_ItemUsed(itemLoader->getItem(itemId)));
				ui->addChatText("GameEvent_ItemUsed sent with Item: " + itemLoader->getItem(itemId)->getName());
			}
			else
			{
				error = true;
			}
		}
		if (error)
		{
			ui->addChatText("Invalid GameEvent Type. Specify either ea, ir or iu");
			ui->addChatText("with appropriate parameters");
		}
		return true;
	}

	return false;
}

void Grpg::attemptQuestCompletions()
{
	map<int, Quest*>* quests = questLoader->getMapQuests();
	for (map<int, Quest*>::iterator i = quests->begin(); i != quests->end(); i++)
	{
		if (i->second->completed() && !i->second->getRewardGiven())
		{
			i->second->gainRewards(ui, player, this);
			//Only give award to 1 quest at a time at most, otherwise the second quest's award
			//window will override the first.
			ui->displayQuestReward(i->second);
			gameEventManager->informEntities();
			break;
		}
	}
}

Entity* Grpg::dropEasterEgg()
{
	//Drop chances directly from https://www.reddit.com/r/hearthstone/comments/2emnxl/drops_chances_rarity_by_11359_hearthstone_expert/
	//(Gold added to normal chances)
	//Decimal places removed, multiply everything by 1000 for full number.
	int commonChance = 70000 + 1470;
	//rand() generates a random number from 0 to RAND_MAX...
	//RAND_MAX is guaranteed to be at least 32767 on any standard library implementation... which is WAY TOO LOW!
	//So we use a different method instead: https://stackoverflow.com/questions/19758694/c-generate-a-random-number-between-0-and-100-000
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100000);

	int randomNumber = dis(gen);
	stringstream ss;
	ss << "Random number generated: " << randomNumber;
	//ui->addChatText(ss.str());
	ss.str("");
	ss << "Common chance: " << commonChance;
	//ui->addChatText(ss.str());
	ss.str("");
	InventoryItem* newItem;
	if (randomNumber > commonChance || easterEggCounter >= 4)
	{
		//You got a rare (or better!)
		//Reset free rare+ counter.
		easterEggCounter = 0;
		int rareChance = 21400 + 1370;
		ss << "Rare chance: " << (rareChance + commonChance);
		//ui->addChatText(ss.str());
		ss.str("");
		if (randomNumber > (commonChance + rareChance))
		{
			//You got an epic (or better!)
			int epicChance = 4280 + 1370;
			ss << "Epic chance: " << (rareChance + commonChance + epicChance);
			//ui->addChatText(ss.str());
			ss.str("");
			if (randomNumber > (commonChance + rareChance + epicChance))
			{
				//WOAH, LEGENDARY!!!!!!!!!!!
				newItem = new InventoryFood(itemLoader->getItem(35), 1, DELICIOUS);
			}
			else
			{
				//Epic confirmed
				newItem = new InventoryItem(itemLoader->getItem(34), 1);
			}
		}
		else
		{
			//Rare confirmed
			newItem = new InventoryBoost(itemLoader->getItem(33), 1);
		}
	}
	else
	{
		//You got a common!
		easterEggCounter++;
		newItem = new InventoryFood(itemLoader->getItem(32), 1, DELICIOUS);
	}
	//Generate new item
	Entity* e = new Entity();
	e->initialize(this, newItem, false);
	return e;
}

void Grpg::saveAnalyticsData()
{
	map<string, map<string, int>> saveData;
	//ui->addChatText("Game saved.");
	//ui->drawWindow("Save Crystal");
	//ui->addTalkText(new ChatInformation("This "))
	saveData["questData"] = questLoader->getQuestData()->getAllValues();
	saveData["skillsData"] = player->getSkillsToSave();
	map<string, int> otherData;
	otherData[player->getInventory()->getInventoryString()] = 0;
	saveData["inventory"] = otherData;
	map<string, int> questData;
	map<int, Quest*>* questList = questLoader->getMapQuests();
	for (map<int, Quest*>::iterator i = questList->begin(); i != questList->end(); ++i)
	{
		questData[i->second->getQuestString()] = i->first;
	}
	saveData["quests"] = questData;
	map<string, int> playerData;
	playerData["deaths"] = player->getTotalDeaths();
	playerData["aidilKills"] = player->getAidilKills();
	playerData["aidilKillTime"] = player->getAidilKillTime();
	if (ui->isNewChatVersion())
	{
		playerData["chat"] = 1;
	}
	else
	{
		playerData["chat"] = 0;
	}
	saveData["player"] = playerData;

	ofstream myfile;
	myfile.open("analytics.txt");
	cereal::JSONOutputArchive output(myfile);
	output(cereal::make_nvp("savedata", saveData));
}