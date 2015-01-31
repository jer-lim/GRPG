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
	SAFE_DELETE(ui);
	SAFE_DELETE(drawManager);
	SAFE_DELETE(mapLoader);
	SAFE_DELETE(personLoader);
	SAFE_DELETE(hitSplat);
	SAFE_DELETE(missSplat);
	SAFE_DELETE(viewport);
	Skill::deleteAllSkills();
	SAFE_DELETE(player);
	SAFE_DELETE(Person::thePlayer);
	SAFE_DELETE(itemLoader);
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
	VECTOR2 startLocation = VECTOR2(4.5*tileNS::CHUNK_WIDTH*tileNS::WIDTH, 4.5*tileNS::CHUNK_HEIGHT*tileNS::HEIGHT);

	// Set viewport
	viewport->setX(startLocation.x);
	viewport->setY(startLocation.y);

	// Load data
	itemLoader = new ItemLoader();
	itemLoader->loadAllItems();
	personLoader->loadAllNPCs();//ml free

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
	if (uiFont->initialize(graphics, 15, false, false, "Arial") == false)
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
	
	player->setX(startLocation.x);
	player->setY(startLocation.y);
	drawManager->addObject(player,3);
	drawManager->addObject(ui, 999);
	// Load and display map, start spawners
	mapLoader->setVictim(player);
	mapLoader->load();

	mouseWasDown = input->getMouseLButton();

	InventoryItem* y = new InventoryItem(itemLoader->getItem(0), 9);
	Entity* e = new Entity();
	e->initialize(this, y, true);//anchored if its an inventory
	//y->initialize(this, true);
	player->getInventory()->addEntityInventoryItem(e);
	//ml free

	//Object test
	//CRIME SCENE
	
	InventoryItem* x = new InventoryItem(itemLoader->getItem(0), 9);
	Entity* newObj = new Entity();
	newObj->initialize(this, x, false);
	//x->initialize(this, false);
	newObj->setX(startLocation.x);
	newObj->setY(startLocation.y);
	drawManager->addObject(newObj, 1);
	//END SCENE
	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Grpg::update()
{
	if(!input->getMouseLButton() && mouseWasDown)
	{
		if (ui->mouseInside(viewport))
		{
			ui->performClick();
		}
		else
		{
			if (mouseOverEntity != nullptr)
			{
				Behavior* topBehavior = mouseOverEntity->getTopMostBehavior();
				if (topBehavior != nullptr)
				{
					mouseOverEntity->getTopMostBehavior()->action();
				}
			}
			else
			{
				VECTOR2 vpCoords = viewport->reverseTranslate(input->getMouseX(), input->getMouseY());

				Point* p = new Point(vpCoords.x, vpCoords.y);
				player->move(p);
				player->setVictim(0);
			}
		}
		mouseWasDown = false;
	}
	
	if (input->getMouseLButton())
	{
		mouseWasDown = true;
	}
	
	map<int, PlayerSkill>::iterator it;
	map<int, PlayerSkill>* playerSkills = player->getSkills();
	for (it = playerSkills->begin(); it != playerSkills->end(); it++)
	{
		//it->second.gainXP(rand()%10);
	}
	//CRIME SCENE/MEMORY LEAK
	mapLoader->update();
	//CRIME SCENE/MEMORY LEAK
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
	if (command.substr(0, 5) == "spawn")
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
