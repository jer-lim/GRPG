#include "grpg.h"
#include "drawManager.h"
#include "mapLoader.h"
#include "tile.h"
#include "PersonLoader.h"
#include <sstream>

//=============================================================================
// Constructor
//=============================================================================
Grpg::Grpg()
{//Mem leak free
	uiFont = new TextDX();
}

//=============================================================================
// Destructor
//=============================================================================
Grpg::~Grpg()
{
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
	//~Game() called afterward
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Grpg::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
	Skill::setupAllSkills();

	mapLoader->loadData();

	// Set viewport
	viewport->setX(startLocation.x);
	viewport->setY(startLocation.y);

	// Load data
	itemLoader = new ItemLoader();
	itemLoader->loadAllItems();
	personLoader->loadAllNPCs(itemLoader);//ml free

	missSplat = new TextureManager();
	missSplat->initialize(graphics, MISS_IMAGE);
	entityNS::miss.initialize(graphics, 0, 0, 0, missSplat);

	hitSplat = new TextureManager();
	hitSplat->initialize(graphics, HIT_IMAGE);
	entityNS::hit.initialize(graphics, 0, 0, 0, hitSplat);

	entityNS::splatText.initialize(graphics, 12, false, false, "Arial");
	entityNS::splatText.setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));

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
	//Init position of inventory inside inventory to init positioning of inventory items upon their adding into them
	player->getInventory()->setXDrawPosition(ui->getTopLeftX());
	player->getInventory()->setYDrawPosition(ui->getTopLeftY());
	
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
	*/
	InventoryItem* x = new InventoryItem(itemLoader->getItem(0), 9);
	Entity* newObj = new Entity();
	newObj->initialize(this, x, false);
	//x->initialize(this, false);
	newObj->setX(startLocation.x);
	newObj->setY(startLocation.y);
	drawManager->addObject(newObj, 2);
	
	//END SCENE
	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Grpg::update()
{
	if (!input->getMouseLButton() && leftMouseWasDown)
	{
		bool inUI = false;
		if (ui->mouseInside(viewport))
		{
			ui->performClick();
			inUI = true;
		}
		//else
		//{
		bool actionSuccess = false;
		if (mouseOverEntity != nullptr)
		{
			Behavior* topBehavior = mouseOverEntity->getTopMostBehavior();
			if (topBehavior != nullptr)
			{
				mouseOverEntity->getTopMostBehavior()->action();
				actionSuccess = true;
				mouseOverEntity = nullptr;
			}
		}
		if (!inUI && !actionSuccess)
		{
			VECTOR2 vpCoords = viewport->reverseTranslate(input->getMouseX(), input->getMouseY());

			Point* p = new Point(vpCoords.x, vpCoords.y);
			player->move(p);
			player->setVictim(0);
		}
		ui->removeWindow();
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
							addMouseOverEntity(it2->second->entity);
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

	//player->setX(player->getX() + 2);
	
	//View is always centered on the player
	viewport->setX(player->getX());
	viewport->setY(player->getY());
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
		pro = true;
		return true;
	}
	else if (command == "shop")
	{
		ui->drawWindow("Solomon's General Store");
		return true;
	}
	else if (command.substr(0, 5) == "spawn")
	{

		std::string enemyToSpawn = command.substr(6);
		
		if (enemyToSpawn == "skeleton")
			NPC::spawn(this, PersonNS::ID_NPC_SKELETON, VECTOR2(player->getX() + 50, player->getY() - 50), player);
		else if (enemyToSpawn == "goblin")
			NPC::spawn(this, PersonNS::ID_NPC_GOBLIN, VECTOR2(player->getX() - 50, player->getY() - 50), player);
		else if (enemyToSpawn == "ogre")
			NPC::spawn(this, PersonNS::ID_NPC_OGRE, VECTOR2(player->getX() - 50, player->getY() - 50), player);
		else if (enemyToSpawn == "undead")
			NPC::spawn(this, PersonNS::ID_NPC_UNDEAD, VECTOR2(player->getX() - 50, player->getY() - 50), player);
		else if (enemyToSpawn == "golem")
			NPC::spawn(this, PersonNS::ID_NPC_GOLEM, VECTOR2(player->getX() - 50, player->getY() - 50), player);
		else if (enemyToSpawn == "dragon")
			NPC::spawn(this, PersonNS::ID_NPC_DRAGON, VECTOR2(player->getX() - 50, player->getY() - 50), player);
		else if (enemyToSpawn == "aidil")
			NPC::spawn(this, PersonNS::ID_NPC_AIDIL, VECTOR2(player->getX() - 50, player->getY() - 50), player);
		else
		{
			ui->addChatText("No such character: " + enemyToSpawn);
			return true;
		}

		return true;
	}

	return false;
}
