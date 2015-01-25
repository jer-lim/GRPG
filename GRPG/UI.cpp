#include "UI.h"

//=============================================================================
// default constructor
//=============================================================================
UI::UI() : Entity()
{
	image.spriteData.width = uiNS::WIDTH;           // size of Ship1
	image.spriteData.height = uiNS::HEIGHT;
	image.spriteData.x = uiNS::X;                   // location on screen
	image.spriteData.y = uiNS::Y;
	image.spriteData.rect.bottom = uiNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = uiNS::WIDTH;
	collisionType = entityNS::NONE;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool UI::initialize(Game *gamePtr)
{
	//UI only have one image
	return(Entity::initialize(gamePtr, image.spriteData.width, image.spriteData.height, 1));
}

//=============================================================================
// draw the player
//=============================================================================
void UI::draw()
{
	Entity::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void UI::update(float frameTime)
{
	Entity::update(frameTime);
}
