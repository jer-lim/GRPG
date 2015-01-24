#include "player.h"

//=============================================================================
// default constructor
//=============================================================================
Player::Player() : Entity()
{
    image.spriteData.width = playerNS::WIDTH;           // size of Ship1
	image.spriteData.height = playerNS::HEIGHT;
	image.spriteData.x = playerNS::X;                   // location on screen
	image.spriteData.y = playerNS::Y;
	image.spriteData.rect.bottom = playerNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = playerNS::WIDTH;
	image.setFrameDelay(playerNS::SHIP_ANIMATION_DELAY);
	image.setFrames(playerNS::SHIP1_START_FRAME, playerNS::SHIP1_END_FRAME);     // set animation frames
	image.setCurrentFrame(playerNS::SHIP1_START_FRAME);
    radius = playerNS::WIDTH/2.0;
    mass = playerNS::MASS;
    collisionType = entityNS::CIRCLE;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Player::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the player
//=============================================================================
void Player::draw()
{
	Entity::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime)
{
	Entity::update(frameTime);
}

//=============================================================================
// damage
//=============================================================================
void Player::damage(WEAPON weapon)
{
}

