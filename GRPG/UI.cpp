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
	game = gamePtr;
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

	//Initalize the health bar
	
	// Calculate the width of the health bar
	// Health bar centralizes in the middle, left maximum chat, right as much as left
	try {
		// top left
		health[0].x = uiNS::chatWidth;
		health[0].y = GAME_HEIGHT - uiNS::healthHeight;
		health[0].z = 0.0f;
		health[0].rhw = 1.0f;
		health[0].color = uiNS::noHealthColor;

		// top right
		health[1].x = uiNS::chatWidth + uiNS::healthWidth;
		health[1].y = GAME_HEIGHT - uiNS::healthHeight;
		health[1].z = 0.0f;
		health[1].rhw = 1.0f;
		health[1].color = uiNS::noHealthColor;

		// bottom right
		health[2].x = uiNS::chatWidth + uiNS::healthWidth;
		health[2].y = GAME_HEIGHT;
		health[2].z = 0.0f;
		health[2].rhw = 1.0f;
		health[2].color = uiNS::noHealthColor;

		// bottom left
		health[3].x = uiNS::chatWidth;
		health[3].y = GAME_HEIGHT;
		health[3].z = 0.0f;
		health[3].rhw = 1.0f;
		health[3].color = uiNS::noHealthColor;

		graphics->createVertexBuffer(health, sizeof health, healthBuffer);
	}
	catch (...) {
		return false;
	}

	//Setup the vector that shows how much health the player has left as much as possible
	try {
		// top left
		availableHealth[0].x = uiNS::chatWidth;
		availableHealth[0].y = GAME_HEIGHT - uiNS::healthHeight;
		availableHealth[0].z = 0.0f;
		availableHealth[0].rhw = 1.0f;
		availableHealth[0].color = uiNS::healthColor;

		// top right
		//availableHealth[1].x = uiNS::chatWidth + healthWidth; //Cannot tell yet
		//availableHealth[1].y = GAME_HEIGHT - uiNS::healthHeight; //Cannot tell yet
		availableHealth[1].z = 0.0f;
		availableHealth[1].rhw = 1.0f;
		availableHealth[1].color = uiNS::healthColor;

		// bottom right
		//availableHealth[2].x = uiNS::chatWidth + healthWidth; //Cannot tell yet
		//availableHealth[2].y = GAME_HEIGHT; //Cannot tell yet
		availableHealth[2].z = 0.0f;
		availableHealth[2].rhw = 1.0f;
		availableHealth[2].color = uiNS::healthColor;

		// bottom left
		availableHealth[3].x = uiNS::chatWidth;
		availableHealth[3].y = GAME_HEIGHT;
		availableHealth[3].z = 0.0f;
		availableHealth[3].rhw = 1.0f;
		availableHealth[3].color = uiNS::healthColor;
	}
	catch (...) {
		return false;
	}

	//Build the chat system
	try {
		// top left
		vtx[0].x = 0;
		vtx[0].y = GAME_HEIGHT - uiNS::chatHeight;
		vtx[0].z = 0.0f;
		vtx[0].rhw = 1.0f;
		vtx[0].color = uiNS::chatColour;

		// top right
		vtx[1].x = uiNS::chatWidth;
		vtx[1].y = GAME_HEIGHT - uiNS::chatHeight;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].color = uiNS::chatColour;

		// bottom right
		vtx[2].x = uiNS::chatWidth;
		vtx[2].y = GAME_HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].color = uiNS::chatColour;

		// bottom left
		vtx[3].x = 0;
		vtx[3].y = GAME_HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].color = uiNS::chatColour;

		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	}
	catch (...) {
		return false;
	}

	//Initalize the chat screen
	//Firstly, create the text rectangle that will draw the chat console
	//onto the screen on the specified locations.
	//These locations will be changed eventually when the text is actually
	//draw, but these values are set first to allow for calculation of
	//row height and number of rows.
	textRect.left = 0;
	textRect.top = 0;

	// sets textRect bottom to height of 1 row
	//When we perform this printing, uiText will modify textRect to only take up the space
	//required to draw that text, has only setting it to the height of one row
	//Note: DT_CALCRECT only sets the rectangle size but does not end up actually drawing the text
	uiText->print("|", textRect, DT_CALCRECT);
	rowHeight = textRect.bottom + 2;    // height of 1 row (+2 is row spacing)
	if (rowHeight <= 0)                      // this should never be true
		rowHeight = 20;                     // force a workable result

	// Find the number of rows that will fit into the height of the chat
	rows = (uiNS::chatHeight) / rowHeight;
	rows -= 1;                              // room for input prompt at bottom
	if (rows <= 0)                          // this should never be true
		rows = 5;                           // force a workable result

	//UI only have one image
	return(Entity::initialize(gamePtr, image.spriteData.width, image.spriteData.height, 1, UI_IMAGE, true));
}

//=============================================================================
// draw the player
//=============================================================================
void UI::draw(Viewport* viewport)
{
	//UI completely ignores viewport =)

	//Fix required otherwise graphics textures drawn will be drawn behind the tiles
	graphics->spriteEnd();
	graphics->spriteBegin();

	graphics->drawQuad(vertexBuffer);       // draw backdrop

	// Display the chat screen

	// set text display rect for one row
	// Defines the text rectangle left and right locations
	textRect.left = (long)(uiNS::tabMargin);
	textRect.right = (long)(textRect.left + uiNS::chatWidth - uiNS::tabMargin);

	// Now set the drawing parts top and bottom.
	//textRect.top = ; // Top doesn't actually need to be set because it will be later set in the for loop
	// -rowHeight is room for input prompt
	// Chat is fixated at the bottom
	textRect.bottom = (long)(GAME_HEIGHT - uiNS::tabMargin - rowHeight);

	// for all rows (max text.size()) from bottom to top
	for (int r = 0; r<rows && r<(int)(text.size()); r++)
	{
		// set text display rect top for this row
		// So the text is drawn from here (Bottom up!)
		textRect.top = textRect.bottom - rowHeight;
		// display one row of text
		uiText->print(text[r], textRect, DT_LEFT);
		// adjust text display rect bottom for next row, moving it up
		textRect.bottom -= rowHeight;
	}

	// display command prompt and current command string
	// set text display rect for prompt
	textRect.bottom = (long)(GAME_HEIGHT- uiNS::tabMargin);
	textRect.top = textRect.bottom - rowHeight;

	std::string prompt = ">";                   // build prompt string
	std::string playerText = input->getTextIn();

	//If something was entered into the game
	if (playerText.length() > 0)
	{
		if (playerText.at(playerText.length() - 1) == '\r')   // if 'Enter' key is pressed
		{
			playerText.erase(playerText.length() - 1);		// erase '\r' from end of command string
			processCommand(playerText);						//Execute the command
		}
	}

	uiText->print(prompt + playerText, textRect, DT_LEFT);      // display prompt and command
	
	if (uiNS::COMBATSTYLE != activeTab)
		drawTab(uiNS::COMBATSTYLE);
	if (uiNS::SKILLS != activeTab)
		drawTab(uiNS::SKILLS);
	if (uiNS::INVENTORY != activeTab)
		drawTab(uiNS::INVENTORY);

	//Now draw the required stuff for the health bar
	//Calculate how much health the player has left
	float healthPercent = player->getHealth() / player->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel();
	if (healthPercent < 0)
	{
		healthPercent = 0;
	}
	try {
		// top right
		availableHealth[1].x = uiNS::chatWidth + uiNS::healthWidth*healthPercent;
		availableHealth[1].y = GAME_HEIGHT - uiNS::healthHeight;

		// bottom right
		availableHealth[2].x = uiNS::chatWidth + uiNS::healthWidth*healthPercent;
		availableHealth[2].y = GAME_HEIGHT;

		graphics->createVertexBuffer(availableHealth, sizeof availableHealth, availableHealthBuffer);
	}
	catch (...) {
		throw new GameError(gameErrorNS::FATAL_ERROR, "Health could not be drawn");
	}

	graphics->drawQuad(healthBuffer);       // draw backdrop
	graphics->drawQuad(availableHealthBuffer);       // draw backdrop

	Entity::draw(viewport);

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
	float topLeftX = getTopLeftX();//getX() - uiNS::WIDTH / 2;
	float topLeftY = getTopLeftY();// getY() - uiNS::HEIGHT / 2;

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
		map<int, InventoryItem*>* slotList = player->getInventory()->getSlotList();
		for (std::map<int, InventoryItem*>::iterator it = slotList->begin(); it != slotList->end(); ++it)
		{
			it->second->getEntity()->draw(nullptr);
			//draw stack here using uiText
			if (it->second->getCurrentStackCount() > 1)//draw stack count
			{
				uiText->print(it->second->getCurrentStackCountString(), (int)(it->second->getEntity()->getX()), (int)(it->second->getEntity()->getY()));
			}
		}
		(slotList) = nullptr;
	}
}

//=============================================================================
// Process console command
// Returns true if processing was done, false otherwise
//=============================================================================
bool UI::processCommand(const std::string commandStr)
{
	input->clearTextIn();                       // clear input line

	if (commandStr.length() == 0)               // if no command entered
		return true;

	//Process the command string for cheat messages
	if (commandStr == "exit")
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Exit command called"));
		return true;
	}

	//Check if the game can process it
	if (game->processCommand(commandStr))
	{
		return true;
	}

	// Valid message, add it to the chat message line
	addChatText(commandStr);
	player->sayMessage(commandStr, uiText);
	return false;								// return command
}

//=============================================================================
// Add text to console
// Only the first line of text in str will be displayed.
//=============================================================================
void UI::addChatText(const std::string &str)     // add text to console
{
	text.push_front(str);                       // add str to deque of text
	while (text.size() > rows)
		text.pop_back();                        // delete oldest line
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void UI::update(float frameTime, Game* gamePtr)
{
}

void UI::performClick()
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

//=============================================================================
// Checks if the mouse is currently over any part of the UI.
// Returns true if mouse is over, false if not
//=============================================================================
bool UI::mouseInside(Viewport vp)
{
	if (Entity::mouseInside(vp))
	{
		return true;
	}

	//UI is always anchored, and so doesn't care about any viewport below

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

	//The game should still care about mouse clicks over the chat; it's a non-interactive game area
	/*
	//Check if the mouse is over the chat
	if (input->getMouseY() > GAME_HEIGHT - uiNS::chatHeight && input->getMouseY() < GAME_HEIGHT &&
		input->getMouseX() > 0 && input->getMouseX() < uiNS::chatWidth)
	{
		return true;
	}*/

	return false;
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void UI::onLostDevice()
{
	uiText->onLostDevice();
	SAFE_RELEASE(vertexBuffer);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void UI::onResetDevice()
{
	uiText->onResetDevice();
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
}

string UI::view(){ return "User Interface"; }
