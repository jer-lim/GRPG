#include "UI.h"

//=============================================================================
// default constructor
//=============================================================================
UI::UI() : Entity()
{
	image.spriteData.width = uiNS::WIDTH;           // size of Ship1
	image.spriteData.height = uiNS::HEIGHT;
	image.spriteData.rect.bottom = uiNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = uiNS::WIDTH;
	image.setCurrentFrame(0);
	image.setFrames(0, 0);
	collisionType = entityNS::NONE;
	image.setFrameDelay(0.2);
	uiText = new TextDX();
}

//=============================================================================
// default destructor
//=============================================================================
UI::~UI()
{
	SAFE_DELETE(uiText);
}

//=============================================================================
// Initialize the User interface.
// Post: returns true if successful, false if failed
//=============================================================================
bool UI::initialize(Game *gamePtr)
{
	// 15 pixel high Arial
	if (uiText->initialize(gamePtr->getGraphics(), 15, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));

	//Also white cause background black
	uiText->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));

	//UI only have one image
	return(Entity::initialize(gamePtr, image.spriteData.width, image.spriteData.height, 1, UI_IMAGE));
}

//=============================================================================
// draw the player
//=============================================================================
void UI::draw()
{
	Entity::draw();

	//Draw all text here so that the image properly appears below them

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

//=============================================================================
// called when graphics device is lost
//=============================================================================
void UI::onLostDevice()
{
	uiText->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void UI::onResetDevice()
{
	uiText->onResetDevice();
}
