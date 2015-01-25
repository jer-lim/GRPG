#include "grpg.h"

//=============================================================================
// Constructor
//=============================================================================
Grpg::Grpg()
{}

//=============================================================================
// Destructor
//=============================================================================
Grpg::~Grpg()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Grpg::initialize(HWND hwnd)
{

	// Load map
	MapLoader mapLoader;
	mapLoader.initialize(this, graphics);
	mapLoader.load();

    Game::initialize(hwnd); // throws GameError

	// Initialise entities
	player = new Player();
	player2 = new Player();
	ui = new UI();

	entityManager = EntityManager();
	if(!player->initialize(this))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the player"));

	if (!player2->initialize(this))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the player"));

	if (!ui->initialize(this))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the user interface"));

	ui->setX(uiNS::X);
	ui->setY(uiNS::Y);
	
	player->setX(GAME_WIDTH/2);
	player->setY(GAME_HEIGHT / 2);
	player2->setX(10);
	player2->setY(10);

	player2->setSpeed(50);
	player2->move(player);

	entityManager.addEntity(player);
	entityManager.addEntity(player2);
	entityManager.addEntity(ui);
	
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Grpg::update()
{
	if(input->getMouseLButton())
	{
		Point* p =  new Point(input->getMouseX(), input->getMouseY());
		player->move(p);
	}

	entityManager.updateAll(frameTime);
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

	entityManager.renderAll();

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Grpg::releaseAll()
{
	entityManager.releaseAll();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Grpg::resetAll()
{
	entityManager.resetAll();
    Game::resetAll();
    return;
}
