#include "UI.h"
#include <map>
#include "playerSkill.h"

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
	image.setFrameDelay(1);
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
bool UI::initialize(Game* gamePtr, Player* p)
{
	player = p;

	// 15 pixel high Arial
	if (uiText->initialize(gamePtr->getGraphics(), uiNS::textSize, true, false, "Arial") == false)
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
	float heightAllowed = uiNS::HEIGHT / 7; //We have 7 skills
	float yLocation = getY() - uiNS::HEIGHT/2;
	map<std::string, PlayerSkill> playerSkills = player->getSkills();
	map<std::string, PlayerSkill>::iterator it;
	for (it = playerSkills.begin(); it != playerSkills.end(); it++)
	{
		//Print the skill text at the center of each location, with 5 px margin: left;
		uiText->print(it->second.getSkill().getName(),
			getX() + 5 - uiNS::WIDTH/2, yLocation + heightAllowed/2 - (uiNS::textSize/2));
		yLocation += heightAllowed;
	}
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
