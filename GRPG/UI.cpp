#include "UI.h"
#include <map>
#include "playerSkill.h"
#include <sstream>
#include "grpg.h"

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
	uiImgTexture = new TextureManager();
	windowTexture = new TextureManager();
	shopTexture = new TextureManager();
	activeTab = uiNS::SKILLS;

	//Not visible till you right click
	rightClickBackground.setVisible(false);
}

//=============================================================================
// default destructor
//=============================================================================
UI::~UI()
{
	onLostDevice();
	SAFE_DELETE(uiText);
	SAFE_DELETE(tabTexture);
	SAFE_DELETE(uiImgTexture);
	SAFE_DELETE(windowTexture);
	SAFE_DELETE(shopRect);
	SAFE_DELETE(shopTexture);

	delete coin;
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
	if (uiText->initialize(graphics, uiNS::textSize, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));

	//init texture
	if (!tabTexture->initialize(graphics, TAB_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing tabs texture"));
	if (!uiImgTexture->initialize(graphics, UI_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing ui_image texture"));
	if (!tabImage.initialize(graphics, 0, 0, 1, tabTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Tabs image could not be initalized"));
	if (!windowTexture->initialize(graphics, WINDOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing window texture"));
	if (!windowImage.initialize(graphics, 0, 0, 1, windowTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Window image could not be initalized"));
	if (!shopTexture->initialize(graphics, SHOP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing Shop Note Texture"));
	if (!shopImage.initialize(graphics, 0, 0, 1, shopTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Shop Image could not be initalized"));	

	//Initalize the shop rectangle that will be used to draw the text at the top of the shop interface
	windowImage.setX(GAME_WIDTH / 2);
	windowImage.setY(GAME_HEIGHT / 2);
	shopRect = new RECT();
	shopRect->left = windowImage.getX() - windowImage.getWidth() / 2;
	shopRect->top = windowImage.getY() - windowImage.getHeight() / 2;
	shopRect->right = shopRect->left + windowImage.getWidth();
	shopRect->bottom = shopRect->top + uiNS::windowXHeight;

	//Also white cause background black
	uiText->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));

	//Initalize the health bar
	if (!health.initialize(graphics, uiNS::chatWidth, GAME_HEIGHT - uiNS::healthHeight, uiNS::healthWidth, uiNS::healthHeight, uiNS::noHealthColor, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Health could not be initalized");
	}
	if (!availableHealth.initialize(graphics, uiNS::chatWidth, GAME_HEIGHT - uiNS::healthHeight, uiNS::healthWidth, uiNS::healthHeight, uiNS::healthColor, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Available Health could not be initalized");
	}
	if (!chatRect.initialize(graphics, 0, GAME_HEIGHT - uiNS::chatHeight, uiNS::chatWidth, uiNS::chatHeight, uiNS::chatColour, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Chat background could not be initalized");
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

	//Initalize a single coin - this is used for all buy transactions
	InventoryItem* x = new InventoryItem(((Grpg*)game)->getItemLoader()->getItem(0), 0);
	coin = new Entity();
	coin->initialize(game, x, false);

	//UI only have one image
	return(Entity::initialize(gamePtr, image.spriteData.width, image.spriteData.height, 1, uiImgTexture, true));
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

	chatRect.draw();

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
	if (uiNS::EQUIPS != activeTab)
		drawTab(uiNS::EQUIPS);
	if (uiNS::QUESTS != activeTab)
		drawTab(uiNS::QUESTS);

	//Now draw the required stuff for the health bar
	//Calculate how much health the player has left
	float healthPercent = player->getHealth() / player->getSkills()->at(skillNS::ID_SKILL_TOUGHNESS).getSkillLevel();
	if (healthPercent < 0)
	{
		healthPercent = 0;
	}
	try {
		availableHealth.initializeRectangle(graphics, uiNS::chatWidth, GAME_HEIGHT - uiNS::healthHeight, uiNS::healthWidth*healthPercent, uiNS::healthHeight, uiNS::healthColor);
	}
	catch (...) {
		throw new GameError(gameErrorNS::FATAL_ERROR, "Health could not be drawn");
	}

	health.draw();
	availableHealth.draw();

	Entity::draw(viewport);

	drawTab(activeTab);

	//Draw all text here so that the image properly appears below them
	drawTabContents(activeTab);

	//Draw the window if required
	if (windowHeader != "")
	{
		windowImage.draw();
		//And draw the text over it
		uiText->print(windowHeader, *shopRect, DT_BOTTOM | DT_CENTER);
		//Draw shop items, if any
		if (items.size() > 0)
		{
			//Draw the text at the bottom first
			VECTOR2 shopNoteLocation = VECTOR2(windowImage.getX(),
				windowImage.getY() + windowImage.getHeight() / 2 - uiNS::windowBottomBorder - shopImage.getHeight()/2);
			shopImage.setX(shopNoteLocation.x);
			shopImage.setY(shopNoteLocation.y);
			shopImage.draw();

			VECTOR2 coordinates = VECTOR2(windowImage.getX() - windowImage.getWidth() / 2 + uiNS::shopLMargin,
										windowImage.getY() - windowImage.getHeight() / 2 + uiNS::shopTMargin);
			coordinates += VECTOR2(itemNS::spriteHeight / 2, itemNS::spriteWidth / 2);
			int originalX = coordinates.x;
			int count = 0; 
			stringstream ss;
			for (vector<Entity* >::iterator it = items.begin(); it != items.end(); ++it)
			{
				Entity* theItem = *it;
				theItem->setX(coordinates.x);
				theItem->setY(coordinates.y);
				theItem->draw(nullptr);
				ss.str("");
				ss << "$" << theItem->getInventoryItem()->getCost();
				//Draw price
				uiText->print(ss.str(),
					theItem->getX() - theItem->getImage()->getHeight()/2, theItem->getY() + theItem->getImage()->getHeight() / 2);
				coordinates.x += itemNS::spriteWidth + uiNS::shopWMargin;
				if (++count % uiNS::shopColMax == 0)
				{
					coordinates.x = originalX;
					coordinates.y += itemNS::spriteHeight + uiNS::shopHMargin;
				}
			}
		}
	}

	// Now draw the right click menu
	if (rightClickBackground.getVisible())
	{
		graphics->spriteEnd();
		graphics->spriteBegin();

		rightClickBackground.draw(uiText);
	}
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
			uiText->print(it->second.getSkill()->getName(),
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
		//playerSkills = nullptr;
	}
	else if (tabNumber == uiNS::INVENTORY)
	{
		//Temporary text
		uiText->print("Inventory", topLeftX + 5, topLeftY + 5);
		map<int, Entity*>* slotList = player->getInventory()->getSlotList();
		for (std::map<int, Entity*>::iterator it = slotList->begin(); it != slotList->end(); ++it)
		{
			//this is actually being drawn twice, once by drawmanager
			it->second->draw(nullptr);
			//draw stack here using uiText
			InventoryItem* ii = it->second->getInventoryItem();
			if (ii->getCurrentStackCount() > 1)//draw stack count
			{
				uiText->print(ii->getCurrentStackCountString(), (int)(it->second->getX()), (int)(it->second->getY()));
			}
		}
		(slotList) = nullptr;
	}
	else if (tabNumber == uiNS::EQUIPS)
	{
		//Temporary text
		uiText->print("Equipment", topLeftX + 5, topLeftY + 5);
		//draw slots
		if (player->getInventory()->getSlotBody() != nullptr)
		{
			player->getInventory()->getSlotBody()->draw(nullptr);
		}
		if (player->getInventory()->getSlotHand() != nullptr)
		{
			player->getInventory()->getSlotHand()->draw(nullptr);
		}
		if (player->getInventory()->getSlotOffHand() != nullptr)
		{
			player->getInventory()->getSlotOffHand()->draw(nullptr);
		}
	}
	else if (tabNumber == uiNS::QUESTS)
	{
		uiText->print("Quests", topLeftX + 5, topLeftY + 5);
		map<int, Quest*>* mapQuests = ((Grpg*)theGame)->getQuestLoader()->getMapQuests();
		for (std::map<int, Quest*>::iterator it = mapQuests->begin(); it != mapQuests->end(); ++it)
		{//draw quests
			it->second->getUIElement()->draw(uiText);
		}
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
	stringstream ss;
	string line = "";
	char tempChar;
	ss << str;
	tempChar = ss.get();
	while (tempChar != EOF){
		if (tempChar == '\n'){
			text.push_front(line);
			line = "";
		}
		else{
			line += tempChar;
		}
		tempChar = ss.get();
	}
	text.push_front(line);
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


//=============================================================================
// Takes note that a click was performed and processes it
// Checking which part of the UI it was over and performing the action
// Returns true if something was processed in the UI
// Returns false if nothing was processed
//=============================================================================
bool UI::performClick()
{
	//Check for right click menu
	if (rightClickBackground.getVisible())
	{
		if (rightClickBackground.mouseOver(input->getMouseX(), input->getMouseY()))
		{
			//Find the behavior option that corresponds to that click
			//And execute that action
			int differenceFromTop = input->getMouseY() - rightClickBackground.getVtx()[0].y;
			RECT* textRect = new RECT();
			textRect->left = 0;
			textRect->top = 0;

			for (std::vector<Behavior*>::iterator it = menus.begin(); it != menus.end(); ++it) {
				Behavior* item = *it;
				uiText->print(item->displayText(), *textRect, DT_CALCRECT);
				if (differenceFromTop <= textRect->bottom)
				{
					//User clicked on this option!
					item->action();
					delete textRect;
					return true;
				}
				differenceFromTop -= textRect->bottom;
			}
			delete textRect;
		}
	}

	//Check if mouse is over any tab
	float tabTopLeftY = getY() - uiNS::HEIGHT / 2 - uiNS::tabHEIGHT * 3 / 4;
	float tabBottomLeftY = getY() - uiNS::HEIGHT / 2;

	if (input->getMouseY() >= tabTopLeftY && input->getMouseY() <= tabBottomLeftY)
	{
		float tabTopLeftX = getX() - uiNS::WIDTH / 2 + uiNS::tabLMargin;
		for (int i = 0; i < uiNS::totalTabs; i++)
		{
			if (input->getMouseX() >= tabTopLeftX && input->getMouseX() <= tabTopLeftX + uiNS::tabWIDTH)
			{
				//Assuming uiNS active tabs possibility are from 1 to totalTabs
				setActiveTab(i + 1);//to add special code
				//activeTab = i+1;
				return true;
			}
			tabTopLeftX += uiNS::tabWIDTH + uiNS::tabMargin;
		}
		// Increase in tab: (tabNumber - 1)*(uiNS::tabWIDTH + uiNS::tabMargin) + uiNS::tabWIDTH / 2);
	}

	//Check for over X icon in window store
	if (windowHeader != "")
	{
		VECTOR2 topRightWindow = VECTOR2(windowImage.getX() + windowImage.getWidth() / 2, windowImage.getY() - windowImage.getHeight() / 2);
		if (input->getMouseX() > topRightWindow.x - uiNS::windowXWidth && input->getMouseX() < topRightWindow.x &&
			input->getMouseY() > topRightWindow.y && input->getMouseY() < topRightWindow.y + uiNS::windowXHeight)
		{
			windowHeader = "";
			return true;
		}
	}
	return false;
}

//=============================================================================
// Checks if the mouse is currently over any part of the UI.
// Returns true if mouse is over, false if not
//=============================================================================
bool UI::mouseInside(Viewport* vp)
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
		for (int i = 0; i < uiNS::totalTabs; i++)
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

	//Check if the mouse is over the right click menu
	if (rightClickBackground.getVisible())
	{
		if (rightClickBackground.mouseOver(input->getMouseX(), input->getMouseY()))
		{
			return true;
		}
	}

	//Check for the window
	if (windowHeader != "")
	{
		if (input->getMouseX() > windowImage.getX() - windowImage.getWidth() / 2 && input->getMouseX() < windowImage.getX() + windowImage.getWidth() / 2 &&
			input->getMouseY() > windowImage.getY() - windowImage.getHeight() / 2 && input->getMouseY() < windowImage.getY() + windowImage.getHeight() / 2)
		{
			return true;
		}
	}

	return false;
}


//Draws a right click 
void UI::setRightClickMenu(vector<Behavior*> behaviors)
{
	menus = behaviors;
	menuTop.x = input->getMouseX();
	menuTop.y = input->getMouseY();

	// Decide how much text should be shown onto the screen
	// Find out the longest text of any behavior
	// The total height
	// And group all the text into a string to be shown
	RECT* textRect = new RECT();
	textRect->left = 0;
	textRect->top = 0;
	int totalHeight = 0;
	int maximumWidth = 0;
	std::string completeText = "";

	for (std::vector<Behavior*>::iterator it = menus.begin(); it != menus.end(); ++it) {
		Behavior* item = *it;
		uiText->print(item->displayText(), *textRect, DT_CALCRECT);
		if (maximumWidth < textRect->right)
		{
			maximumWidth = textRect->right;
		}
		totalHeight += textRect->bottom;
		completeText += item->displayText() + "\n";
	}
	delete textRect;

	rightClickBackground.initialize(graphics, menuTop.x, menuTop.y, maximumWidth, totalHeight, uiNS::rightClickBG, completeText);
	rightClickBackground.setVisible(true);
}

void UI::setShopItems(vector<Entity* > i)
{
	//Add sell behavior to the player's inventory
	//We can leave the rest there, why not allow the player to eat, drop or whatever
	//while shopping?

	vector<Entity*> playerInventory = player->getInventory()->getVectorItems();
	for (vector<Entity*>::iterator it = playerInventory.begin(); it != playerInventory.end(); ++it)
	{
		Entity* theItem = *it;
		//don't add sell behavior for gold
		if (theItem->getInventoryItem()->getItem()->getName() != "Coin")
		{
			theItem->sellBehavior = new SellBehavior(player, theItem, (Grpg*)game);
			theItem->setupVectorActiveBehaviors();
		}
	}

	for (vector<Entity*>::iterator it = i.begin(); it != i.end(); ++it)
	{
		//Setup behaviors
		Entity* theItem = *it;
		//Ensure you can't go and mess around with items in the Shopkeeper's inventory
		SAFE_DELETE(theItem->dropBehavior);
		SAFE_DELETE(theItem->pickupBehavior);
		SAFE_DELETE(theItem->eatBehavior);
		SAFE_DELETE(theItem->cookBehavior);
		if (theItem->buyBehavior == nullptr)
		{
			theItem->buyBehavior = new BuyBehavior(player, theItem, coin, (Grpg*)game);
		}
		theItem->setupVectorActiveBehaviors();
	}
	items = i;
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void UI::onLostDevice()
{
	uiText->onLostDevice();
	chatRect.onLostDevice();
	health.onLostDevice();
	availableHealth.onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void UI::onResetDevice()
{
	uiText->onResetDevice();
	chatRect.onResetDevice();
	health.onResetDevice();
	availableHealth.onResetDevice();
}
