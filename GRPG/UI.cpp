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

	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");
	text.push_front("Abaliszucbowsuefbszoubcozsuidvboaswzidbvozsudv");

	//UI only have one image
	return(Entity::initialize(gamePtr, image.spriteData.width, image.spriteData.height, 1, UI_IMAGE));
}

//=============================================================================
// draw the player
//=============================================================================
void UI::draw()
{
	graphics->drawQuad(vertexBuffer);       // draw backdrop

	// Display the chat screen
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
	uiText->print("|", textRect, DT_CALCRECT);
	int rowHeight = textRect.bottom + 2;    // height of 1 row (+2 is row spacing)
	if (rowHeight <= 0)                      // this should never be true
		rowHeight = 20;                     // force a workable result

	// Find the number of rows that will fit into the height of the chat
	int rows = (uiNS::chatHeight) / rowHeight;
	rows -= 2;                              // room for input prompt at bottom
	if (rows <= 0)                          // this should never be true
		rows = 5;                           // force a workable result
	maximumRows = rows;

	// set text display rect for one row
	// Defines the text rectangle left and right locations
	textRect.left = (long)(uiNS::tabMargin);
	textRect.right = (long)(textRect.left + uiNS::chatWidth - uiNS::tabMargin);

	// Now set the drawing parts top and bottom.
	//textRect.top = ; // Top doesn't actually need to be set because it will be later set in the for loop
	// -2*rowHeight is room for input prompt
	// Chat is fixated at the bottom
	textRect.bottom = (long)(GAME_HEIGHT - uiNS::tabMargin - 2 * rowHeight);

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
// Process console command
// Returns true if processing was done, false otherwise
//=============================================================================
bool UI::processCommand(const std::string commandStr)
{
	//check for Esc key
	if (input->wasKeyPressed(ESC_KEY))
	{
		input->clearTextIn();                       // clear input line
		return false;
	}

	if (commandStr.length() == 0)               // if no command entered
		return true;

	addChatText(commandStr);
	input->clearTextIn();                       // clear input line
	return true;								// return command
}

//=============================================================================
// Add text to console
// Only the first line of text in str will be displayed.
//=============================================================================
void UI::addChatText(const std::string &str)     // add text to console
{
	text.push_front(str);                       // add str to deque of text
	if (text.size() > maximumRows)
		text.pop_back();                        // delete oldest line
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
