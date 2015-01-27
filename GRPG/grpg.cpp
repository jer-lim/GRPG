#include "grpg.h"
#include <sstream>

//=============================================================================
// Constructor
//=============================================================================
Grpg::Grpg()
{
	uiFont = new TextDX();
}

//=============================================================================
// Destructor
//=============================================================================
Grpg::~Grpg()
{
    releaseAll();           // call onLostDevice() for every graphics item
	SAFE_DELETE(uiFont);
	SAFE_DELETE(drawManager);
	SAFE_DELETE(mapLoader);
	SAFE_DELETE(itemLoader);
	SAFE_DELETE(personLoader);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Grpg::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

	VECTOR2 startLocation = VECTOR2(3.5*tileNS::CHUNK_WIDTH*tileNS::WIDTH, 5.5*tileNS::CHUNK_HEIGHT*tileNS::HEIGHT);

	viewport = new Viewport(this, startLocation.x, startLocation.y, GAME_WIDTH, GAME_HEIGHT);

	drawManager = new DrawManager();
	drawManager->initialize(viewport);

	// Load map
	mapLoader = new MapLoader();
	mapLoader->initialize(this, drawManager, viewport);
	mapLoader->load();

	// Load data
	itemLoader = new ItemLoader();
	itemLoader->loadAllItems();
	personLoader = new PersonLoader();
	personLoader->loadAllNPCs();

	// initialize DirectX fonts
	// 15 pixel high Arial
	if (uiFont->initialize(graphics, 15, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));

	// Initialise entities
	player = new Player();
	ui = new UI();

	if(!player->initialize(this))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the player"));

	if (!ui->initialize(this, player, input))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the user interface"));

	ui->setX(uiNS::X);
	ui->setY(uiNS::Y);
	
	player->setX(startLocation.x);
	player->setY(startLocation.y);

	drawManager->addObject(player);
	drawManager->addObject(ui, 999);
	
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Grpg::update()
{
	if(input->getMouseLButton() && !ui->mouseInside())
	{
		VECTOR2 vpCoords = viewport->reverseTranslate(input->getMouseX(), input->getMouseY());
		Point* p =  new Point(vpCoords.x, vpCoords.y);
		player->move(p);
	}

	map<int, PlayerSkill>::iterator it;
	map<int, PlayerSkill>* playerSkills = player->getSkills();
	for (it = playerSkills->begin(); it != playerSkills->end(); it++)
	{
		it->second.gainXP(rand()%10);
	}
	mapLoader->update();
	drawManager->updateAll(frameTime);

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
		Entity* enemy = new Entity();

		std::string enemyToSpawn = command.substr(6);
		
		if (enemyToSpawn == "skeleton")
			enemy->initialize(this, personLoader->getNPC(PersonNS::ID_NPC_SKELETON));
		else if (enemyToSpawn == "dragon")
			enemy->initialize(this, personLoader->getNPC(PersonNS::ID_NPC_DRAGON));
		else if (enemyToSpawn == "aidil")
			enemy->initialize(this, personLoader->getNPC(PersonNS::ID_NPC_AIDIL));
		else
		{
			ui->addChatText("No such character: " + enemyToSpawn);
			return true;
		}

		enemy->setX(player->getX() - 50);
		enemy->setY(player->getY() - 50);

		enemy->setVictim(player);

		drawManager->addObject(enemy, 1);

		return true;
	}

	return false;
}
