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
    Game::initialize(hwnd); // throws GameError

	if(!playerTexture.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing player texture"));

	if(!player.initialize(this, playerNS::WIDTH, playerNS::HEIGHT,	playerNS::TEXTURE_COLS, &playerTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing the player"));
	
	player.setFrames(playerNS::SHIP1_START_FRAME, playerNS::SHIP1_END_FRAME);
	player.setCurrentFrame(playerNS::SHIP1_START_FRAME);
	player.setX(GAME_WIDTH/2);
	player.setY(GAME_HEIGHT/2);
	

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Grpg::update()
{
	if(input->getMouseLButton())
	{
		int xVelocity, yVelocity;
		if(input->getMouseX() < player.getX())
		{
			xVelocity = playerNS::SPEED * -1;
		}
		else if(input->getMouseX() > player.getX())
		{
			xVelocity = playerNS::SPEED;
		}

		if(input->getMouseY() < player.getY())
		{
			yVelocity = playerNS::SPEED * -1;
		}
		else if(input->getMouseY() > player.getY())
		{
			yVelocity = playerNS::SPEED;
		}
		player.setVelocity(VECTOR2(xVelocity, yVelocity));
	}

	player.update(frameTime);
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

	player.draw();

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Grpg::releaseAll()
{
    //gameTextures.onLostDevice();
	playerTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Grpg::resetAll()
{
    //gameTextures.onResetDevice();
	playerTexture.onResetDevice();
    Game::resetAll();
    return;
}
