#include "grpg.h"

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
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Grpg::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
	entityManager = EntityManager();

	// Load map
	MapLoader mapLoader;
	mapLoader.initialize(this, graphics, &entityManager);
	mapLoader.load();

	// initialize DirectX fonts
	// 15 pixel high Arial
	if (uiFont->initialize(graphics, 15, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));

	// Initialise entities
	player = new Player();
	player2 = new Player();
	ui = new UI();

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

	player2->setSpeed(90);
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
	uiFont->print("Move to location", 5, 0); //Feel free to use this text for any debugging thing

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Grpg::releaseAll()
{
	entityManager.releaseAll();
	uiFont->onLostDevice();
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
	uiFont->onResetDevice();
    Game::resetAll();
    return;
}
