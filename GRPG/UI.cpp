#include "UI.h"
#include <map>
#include "playerSkill.h"
#include <sstream>

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
	tabTexture = new TextureManager();
	activeTab = uiNS::SKILLS;
}

//=============================================================================
// default destructor
//=============================================================================
UI::~UI()
{
	SAFE_DELETE(uiText);
	onLostDevice();
}

//=============================================================================
// Initialize the User interface.
// Post: returns true if successful, false if failed
//=============================================================================
bool UI::initialize(Game* gamePtr, Player* p, Input *in)
{
	player = p;
	input = in;
	graphics = gamePtr->getGraphics();

	// 15 pixel high Arial
	if (uiText->initialize(graphics, uiNS::textSize, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));

	//init texture
	if (!tabTexture->initialize(graphics, TAB_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing tabs texture"));

	if (!tabImage.initialize(graphics, 0, 0, 1, tabTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Tabs image could not be initalized"));

	//Also white cause background black
	uiText->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));

	//Build the chat system
	try {
		// top left
		vtx[0].x = x;
		vtx[0].y = y;
		vtx[0].z = 0.0f;
		vtx[0].rhw = 1.0f;
		vtx[0].color = uiNS::chatColour;

		// top right
		vtx[1].x = x + uiNS::chatWidth;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].color = uiNS::chatColour;

		// bottom right
		vtx[2].x = x + uiNS::chatWidth;
		vtx[2].y = y + uiNS::chatHeight;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].color = uiNS::chatColour;

		// bottom left
		vtx[3].x = x;
		vtx[3].y = y + uiNS::chatHeight;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].color = uiNS::chatColour;

		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	}
	catch (...) {
		return false;
	}

	//UI only have one image
	return(Entity::initialize(gamePtr, image.spriteData.width, image.spriteData.height, 1, UI_IMAGE));
}

//=============================================================================
// draw the player
//=============================================================================
void UI::draw()
{
	if (uiNS::COMBATSTYLE != activeTab)
		drawTab(uiNS::COMBATSTYLE);
	if (uiNS::SKILLS != activeTab)
		drawTab(uiNS::SKILLS);
	if (uiNS::INVENTORY != activeTab)
		drawTab(uiNS::INVENTORY);

	Entity::draw();

	drawTab(activeTab);

	//Draw all text here so that the image properly appears below them
	drawTabContents(activeTab);
}

//=============================================================================
// drawTab
// Draws the specified tab number onto the screen on the correct location
// Also see drawTabContents
//=============================================================================
void UI::drawTab(int tabNumber)
{
	tabImage.setX(getX() - uiNS::WIDTH/2 + uiNS::tabLMargin + (tabNumber-1)*(uiNS::tabWIDTH + uiNS::tabMargin) + uiNS::tabWIDTH/2);
	tabImage.setY(getY() - uiNS::HEIGHT / 2 - uiNS::tabHEIGHT / 4);

	tabImage.draw();
}

//=============================================================================
// drawTabContents
// Draws the specified tab contents onto the screen on the correct location
// Also see drawTab
//=============================================================================
void UI::drawTabContents(int tabNumber)
{
	float topLeftX = getX() - uiNS::WIDTH / 2;
	float topLeftY = getY() - uiNS::HEIGHT / 2;

	if (tabNumber == uiNS::COMBATSTYLE)
	{
		uiText->print("Combat styles", topLeftX + 5, topLeftY + 5);
	}
	else if (tabNumber == uiNS::SKILLS)
	{
		float heightAllowed = uiNS::HEIGHT / 7; //We have 7 skills
		map<int, PlayerSkill>* playerSkills = player->getSkills();
		map<int, PlayerSkill>::iterator it;
		stringstream skillLevel;
		for (it = playerSkills->begin(); it != playerSkills->end(); it++)
		{
			//Print the skill text at the center of each location, with 5 px margin: left;
			uiText->print(it->second.getSkill().getName(),
				topLeftX + 5, topLeftY + heightAllowed / 2 - (uiNS::textSize / 2));
			//Check skill level and append a 0 in front if needed
			if (it->second.getSkillLevel() < 10)
			{
				skillLevel << "0" << it->second.getSkillLevel();
			}
			else
			{
				skillLevel << it->second.getSkillLevel();
			}
			//Print level
			uiText->print(skillLevel.str() + "/99",
				getX() + 40, topLeftY + heightAllowed / 2 - (uiNS::textSize / 2));

			skillLevel.str("");

			topLeftY += heightAllowed;
		}
	}
	else if (tabNumber == uiNS::INVENTORY)
	{
		//Temporary text
		uiText->print("Inventory", topLeftX + 5, topLeftY + 5);
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

	if (input->getMouseLButton())
	{
		//Check if mouse is over any tab
		float tabTopLeftY = getY() - uiNS::HEIGHT / 2 - uiNS::tabHEIGHT * 3 / 4;
		float tabBottomLeftY = getY() - uiNS::HEIGHT / 2;

		if (input->getMouseY() >= tabTopLeftY && input->getMouseY() <= tabBottomLeftY)
		{
			float tabTopLeftX = getX() - uiNS::WIDTH / 2 + uiNS::tabLMargin;
			// Increase in tab: (tabNumber - 1)*(uiNS::tabWIDTH + uiNS::tabMargin) + uiNS::tabWIDTH / 2);
			if (input->getMouseX() >= tabTopLeftX && input->getMouseX() <= tabTopLeftX + uiNS::tabWIDTH)
			{
				activeTab = uiNS::COMBATSTYLE;
			}
			tabTopLeftX += uiNS::tabWIDTH + uiNS::tabMargin;
			if (input->getMouseX() >= tabTopLeftX && input->getMouseX() <= tabTopLeftX + uiNS::tabWIDTH)
			{
				activeTab = uiNS::SKILLS;
			}
			tabTopLeftX += uiNS::tabWIDTH + uiNS::tabMargin;
			if (input->getMouseX() >= tabTopLeftX && input->getMouseX() <= tabTopLeftX + uiNS::tabWIDTH)
			{
				activeTab = uiNS::INVENTORY;
			}
		}
	}
}

//=============================================================================
// Checks if the mouse is currently over any part of the UI.
// Returns true if mouse is over, false if not
//=============================================================================
bool UI::mouseInside()
{
	if (Entity::mouseInside())
	{
		return true;
	}

	//Check if mouse is over any tab
	float tabTopLeftY = getY() - image.getHeight() / 2 - uiNS::tabHEIGHT * 3 / 4;
	float tabBottomLeftY = getY() - image.getHeight() / 2;

	if (input->getMouseY() > tabTopLeftY && input->getMouseY() < tabBottomLeftY)
	{
		float tabTopLeftX = getX() - uiNS::WIDTH / 2 + uiNS::tabLMargin;
		for (int i = 0; i < 3; i++)
		{
			// Increase in tab: (tabNumber - 1)*(uiNS::tabWIDTH + uiNS::tabMargin) + uiNS::tabWIDTH / 2);
			if (input->getMouseX() >= tabTopLeftX && input->getMouseX() <= tabTopLeftX + uiNS::tabWIDTH)
			{
				return true;
			}
			tabTopLeftX += uiNS::tabWIDTH + uiNS::tabMargin;
		}
	}
	return false;
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
