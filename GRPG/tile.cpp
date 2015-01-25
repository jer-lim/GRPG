#include "tile.h"

//=============================================================================
// default constructor
//=============================================================================
Tile::Tile() : Entity()
{
    image.spriteData.width = tileNS::WIDTH;           // size of tile
	image.spriteData.height = tileNS::HEIGHT;
	image.spriteData.x = tileNS::X;                   // location on screen
	image.spriteData.y = tileNS::Y;
	image.spriteData.rect.bottom = tileNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = tileNS::WIDTH;
	image.setFrameDelay(tileNS::ANIMATION_DELAY);
	image.setFrames(tileNS::START_FRAME, tileNS::END_FRAME);     // set animation frames
	image.setCurrentFrame(tileNS::START_FRAME);
	radius = tileNS::WIDTH / 2.0;
	mass = tileNS::MASS;
    collisionType = entityNS::NONE;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Tile::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager* tm)
{
    return(Entity::initialize(gamePtr, width, height, ncols, tm));
}

//=============================================================================
// draw the player
//=============================================================================
void Tile::draw()
{
	Entity::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Tile::update(float frameTime)
{
	Entity::update(frameTime);
}
