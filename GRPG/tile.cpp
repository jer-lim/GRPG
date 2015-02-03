#include "tile.h"
#include "grpg.h"
#include "StoveMoveBehavior.h"

//=============================================================================
// default constructor
//=============================================================================
Tile::Tile(char i, bool collidable) : Entity()
{
	id = i;
    image.spriteData.width = tileNS::WIDTH;           // size of tile
	image.spriteData.height = tileNS::HEIGHT;
	image.spriteData.rect.bottom = tileNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = tileNS::WIDTH;
	image.spriteData.x = tileNS::X;
	image.spriteData.y = tileNS::Y;
	image.setCurrentFrame(0);
	image.setFrames(0, 0);
	if (collidable) collisionType = entityNS::BOX;
	else collisionType = entityNS::NONE;
	image.setFrameDelay(0.2f);
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
/*bool Tile::initialize(Game *gamePtr, const char image[])
{
	return(Entity::initialize(gamePtr, tileNS::WIDTH, tileNS::HEIGHT, 1, image));
}*/

bool Tile::initialize(Game *gamePtr, TextureManager* tm)
{
	if (/*stove*/false)
	{
		stoveBehavior = new StoveMoveBehavior(((Grpg*)gamePtr)->getPlayer(), this, ((Grpg*)gamePtr)->getUI());
	}
	return(Entity::initialize(gamePtr, tileNS::WIDTH, tileNS::HEIGHT, 1, tm));
}

//=============================================================================
// draw the player
//=============================================================================
void Tile::draw(Viewport* viewport)
{
	Entity::draw(viewport);
}

void Tile::update(){}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Tile::update(float frameTime, Game* gamePtr){
	image.update(frameTime);
}