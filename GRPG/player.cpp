#include "player.h"

//=============================================================================
// default constructor
//=============================================================================
Player::Player() : Entity()
{
    spriteData.width = playerNS::WIDTH;           // size of Ship1
    spriteData.height = playerNS::HEIGHT;
    spriteData.x = playerNS::X;                   // location on screen
    spriteData.y = playerNS::Y;
    spriteData.rect.bottom = playerNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = playerNS::WIDTH;
    frameDelay = playerNS::SHIP_ANIMATION_DELAY;
    startFrame = playerNS::SHIP1_START_FRAME;     // first frame of ship animation
    endFrame     = playerNS::SHIP1_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
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
    Image::draw();              // draw player
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

