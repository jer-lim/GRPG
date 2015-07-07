//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "UI.h"
#include <map>
#include "playerSkill.h"
#include <sstream>
#include "grpg.h"
#include "Quest.h"
#include <fstream>
#include <iostream>

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
	newTextRect = new RECT();
	uiText = new TextDX();
	levelUpFont = new TextDX();
	skillsText = new TextDX();
	tabTexture = new TextureManager();
	uiImgTexture = new TextureManager();
	windowTexture = new TextureManager();
	shopTexture = new TextureManager();
	mainMenuTexture = new TextureManager();
	checkboxTexture = new TextureManager();
	checkboxSelectedTexture = new TextureManager();
	chatHistoryTexture = new TextureManager();
	levelUpTexture = new TextureManager();
	levelUpImage = new Image();
	activeTab = uiNS::SKILLS;
	questToDisplay = nullptr;
	showHealth = false;
	//Choose a chat version based on the text file
	string line;
	ifstream chatVersion("grpg.txt");
	if (chatVersion.is_open())
	{
		getline(chatVersion, line);
		newChatVersion = line == "true";
		chatVersion.close();
	}
	else //Generate a random value for newchatversion, save it for future runs
	{
		newChatVersion = rand() % 2 == 0;
		ofstream myfile;
		myfile.open("grpg.txt");
		if (newChatVersion)
		{
			myfile << "true";
		}
		else
		{
			myfile << "false";
		}
		myfile.close();
	}

	showChatHistory = false;

	//Not visible till you right click
	rightClickBackground.setVisible(false);
}

//=============================================================================
// default destructor
//=============================================================================
UI::~UI()
{
	removeAllChatData();
	onLostDevice();
	SAFE_DELETE(uiText);
	SAFE_DELETE(levelUpFont);
	SAFE_DELETE(skillsText);
	SAFE_DELETE(tabTexture);
	SAFE_DELETE(uiImgTexture);
	SAFE_DELETE(windowTexture);
	SAFE_DELETE(shopRect);
	SAFE_DELETE(shopTexture);
	SAFE_DELETE(mainMenuTexture);
	SAFE_DELETE(checkboxTexture);
	SAFE_DELETE(checkboxSelectedTexture);
	SAFE_DELETE(chatHistoryTexture);
	SAFE_DELETE(levelUpTexture);
	SAFE_DELETE(levelUpImage);
	SAFE_DELETE(newTextRect);
	for (int i = 0; i < tabTextures.size(); i++)
	{
		delete tabTextures[i];
	}
	for (int i = 0; i < tabImages.size(); i++)
	{
		delete tabImages[i];
	}

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
	//Entity's player variable as well. I know, why are there 2? Sigh.
	thePlayer = player;
	input = in;
	graphics = gamePtr->getGraphics();

	darkRealmVision.initialize(graphics, 0, 0, GAME_WIDTH, GAME_HEIGHT, uiNS::darkRealmColour, "");

	// 15 pixel high Arial
	if (uiText->initialize(graphics, uiNS::textSize, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing UI Font"));
	if (levelUpFont->initialize(graphics, uiNS::textSize, true, true, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Level up Font"));
	if (skillsText->initialize(graphics, uiNS::skillsSize, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Skills font"));

	//init texture
	if (!tabTexture->initialize(graphics, TAB_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing tabs texture"));
	if (!uiImgTexture->initialize(graphics, UI_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing ui_image texture"));
	if (!tabImage.initialize(graphics, 0, 0, 1, tabTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Tabs image could not be initalized"));
	if (!windowTexture->initialize(graphics, WINDOW_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing window texture"));
	if (!mainMenuTexture->initialize(graphics, uiNS::mainMenuLocation))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing main menu texture"));
	if (!checkboxTexture->initialize(graphics, uiNS::checkboxImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing checkbox texture"));
	if (!checkboxSelectedTexture->initialize(graphics, uiNS::checkboxSelectedImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing checkbox selected texture"));
	if (!chatHistoryTexture->initialize(graphics, uiNS::chatHistoryImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing chat history texture"));
	if (!levelUpTexture->initialize(graphics, skillLevelUpNS::arrowLocation))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing up arrow texture"));
	if (!windowImage.initialize(graphics, 0, 0, 1, windowTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Window image could not be initalized"));
	if (!shopTexture->initialize(graphics, SHOP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing Shop Note Texture"));
	if (!shopImage.initialize(graphics, 0, 0, 1, shopTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Shop Image could not be initalized"));	
	if (!mainMenuImage.initialize(graphics, 0, 0, 1, mainMenuTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Main menu Image could not be initalized"));
	if (!checkboxImage.initialize(graphics, uiNS::optionWidth, uiNS::optionHeight, 1, checkboxTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Checkbox Image could not be initalized"));
	if (!checkboxSelectedImage.initialize(graphics, uiNS::optionWidth, uiNS::optionHeight, 1, checkboxSelectedTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Checkbox selected Image could not be initalized"));
	if (!chatHistoryImage.initialize(graphics, 0, 0, 1, chatHistoryTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Chat History Image could not be initalized"));
	if (!levelUpImage->initialize(graphics, skillLevelUpNS::imageWidth, skillLevelUpNS::imageHeight, 1, levelUpTexture, true))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Level up Image could not be initalized"));
	
	TextureManager* optionsTexture = new TextureManager();
	if (!optionsTexture->initialize(graphics, uiNS::optionsImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing options texture"));
	tabTextures.push_back(optionsTexture);
	Image* optionsImage = new Image();
	if (!optionsImage->initialize(graphics, 0, 0, 1, optionsTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Options Image could not be initalized"));
	tabImages.push_back(optionsImage);

	TextureManager* skillsTexture = new TextureManager();
	if (!skillsTexture->initialize(graphics, uiNS::skillsImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing skills texture"));
	tabTextures.push_back(skillsTexture);
	Image* skillsImage = new Image();
	if (!skillsImage->initialize(graphics, 0, 0, 1, skillsTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Skills Image could not be initalized"));
	tabImages.push_back(skillsImage);

	TextureManager* inventoryTexture = new TextureManager();
	if (!inventoryTexture->initialize(graphics, uiNS::inventoryImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing inventory texture"));
	tabTextures.push_back(inventoryTexture);
	Image* inventoryImage = new Image();
	if (!inventoryImage->initialize(graphics, 0, 0, 1, inventoryTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Inventory Image could not be initalized"));
	tabImages.push_back(inventoryImage);

	TextureManager* equipmentTexture = new TextureManager();
	if (!equipmentTexture->initialize(graphics, uiNS::equipmentImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing equipment texture"));
	tabTextures.push_back(equipmentTexture);
	Image* equipmentImage = new Image();
	if (!equipmentImage->initialize(graphics, 0, 0, 1, equipmentTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Equipment Image could not be initalized"));
	tabImages.push_back(equipmentImage);

	TextureManager* questTexture = new TextureManager();
	if (!questTexture->initialize(graphics, uiNS::questsImage))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing quest texture"));
	tabTextures.push_back(questTexture);
	Image* questImage = new Image();
	if (!questImage->initialize(graphics, 0, 0, 1, questTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Quests Image could not be initalized"));
	tabImages.push_back(questImage);

	mainMenuImage.setX(GAME_WIDTH / 2);
	mainMenuImage.setY(GAME_HEIGHT / 2);
	showMainMenu = true;

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
	levelUpFont->setFontColor(graphicsNS::BLACK);
	skillsText->setFontColor(SETCOLOR_ARGB(255, 255, 255, 255));

	//Initalize the health bar
	if (!health.initialize(graphics, uiNS::chatWidth, GAME_HEIGHT - uiNS::healthHeight, uiNS::healthWidth, uiNS::healthHeight, uiNS::noHealthColor, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Health could not be initalized");
	}
	if (!availableHealth.initialize(graphics, uiNS::chatWidth, GAME_HEIGHT - uiNS::healthHeight, uiNS::healthWidth, uiNS::healthHeight, uiNS::healthColor, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Available Health could not be initalized");
	}
	if (!newChatVersion)
	{
		if (!chatRect.initialize(graphics, 0, GAME_HEIGHT - uiNS::chatHeight, uiNS::chatWidth, uiNS::chatHeight, uiNS::chatColour, ""))
		{
			throw new GameError(gameErrorNS::FATAL_ERROR, "Chat background could not be initalized");
		}
	}
	else
	{
		if (!chatRect.initialize(graphics, 0, 0, uiNS::chatWidth, GAME_HEIGHT, graphicsNS::BLACK, ""))
		{
			throw new GameError(gameErrorNS::FATAL_ERROR, "Chat background could not be initalized");
		}
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
	if (newChatVersion)
	{
		rows = GAME_HEIGHT / rowHeight;
	}
	else
	{
		rows = uiNS::chatHeight / rowHeight;
	}
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
	if (lastLevelUpSkill != nullptr)
	{
		lastLevelUpSkill->draw(viewport);
	}
	//UI completely ignores viewport =)

	//Always visible regardless of realm
	setIsInDarkRealm(thePlayer->inDarkRealm());

	//Fix required otherwise graphics textures drawn will be drawn behind the tiles
	graphics->spriteEnd();
	graphics->spriteBegin();

	if (!newChatVersion)
	{
		chatRect.draw();
	}

	if (player->inDarkRealm())
	{
		darkRealmVision.draw();
	}

	// Display the chat screen
	if (!newChatVersion || showChatHistory)
	{
		DWORD oldFontColor;
		if (showChatHistory)
		{
			chatHistoryImage.draw();
			oldFontColor = uiText->getFontColor();
			uiText->setFontColor(graphicsNS::BLACK);
		}
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
		textRect.bottom = (long)(GAME_HEIGHT - uiNS::tabMargin);
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

		if (showChatHistory)
		{
			uiText->setFontColor(oldFontColor);
		}
	}
	
	if (uiNS::OPTIONS != activeTab)
		drawTab(uiNS::OPTIONS);
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

	if (showHealth)
	{
		health.draw();
		availableHealth.draw();
	}

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
			//Don't draw for now cause graphics is ripped from RuneScape
			/*VECTOR2 shopNoteLocation = VECTOR2(windowImage.getX(),
			windowImage.getY() + windowImage.getHeight() / 2 - uiNS::windowBottomBorder - shopImage.getHeight()/2);
			shopImage.setX(shopNoteLocation.x);
			shopImage.setY(shopNoteLocation.y);
			shopImage.draw();*/

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
					theItem->getX() - theItem->getImage()->getHeight() / 2, theItem->getY() + theItem->getImage()->getHeight() / 2);
				coordinates.x += itemNS::spriteWidth + uiNS::shopWMargin;
				if (++count % uiNS::shopColMax == 0)
				{
					coordinates.x = originalX;
					coordinates.y += itemNS::spriteHeight + uiNS::shopHMargin;
				}
			}
		}
		/*else if (windowHeader.find("Quest: ") == 0)
		{//Quest header
			Quest* displayedQuest = ((Grpg*)theGame)->getQuestLoader()->getQuest(((Grpg*)theGame)->getQuestLoader()->getCurrentlyShownIndex());
			//draw description
			uiText->print(displayedQuest->getdescript(), windowImage.getX() - windowImage.getWidth() / 2 + uiNS::shopLMargin, windowImage.getY() - windowImage.getHeight() / 2 + uiNS::shopTMargin);
			//draw number of times completed
			stringstream ss;
			//ss << displayedQuest->getQuestCondition()->getCurrentCount() << " / " << displayedQuest->getQuestCondition()->getCountRequirement();
			if (displayedQuest->completed())
				ss << "\n" << "Quest completed! Head to the Quest Board for your reward!";
			else
				ss << "\n" << "Quest is currently not finished.";
			uiText->print(ss.str(), windowImage.getX() - windowImage.getWidth() / 2 + uiNS::shopLMargin, windowImage.getY() + windowImage.getHeight() / 2 - uiNS::windowBottomBorder * 4 - shopImage.getHeight() / 2);
		}*/
		//Display quest reward, if any
		else if (questToDisplay != nullptr)
		{
			float heightGiven = windowImage.getHeight() - uiNS::windowXHeight - uiNS::windowBottomBorder - uiNS::talkMargin;
			float widthGiven = windowImage.getWidth() - uiNS::windowLRMargin - uiNS::windowLRMargin;

			RECT* testRect = new RECT();
			testRect->left = 0;
			testRect->right = widthGiven;
			testRect->top = 0;

			RECT* actualRect = new RECT();
			actualRect->left = windowImage.getX() - windowImage.getWidth() / 2 + uiNS::windowLRMargin;
			actualRect->right = actualRect->left + widthGiven;
			actualRect->top = windowImage.getY() - windowImage.getHeight() / 2 + uiNS::windowXHeight + uiNS::talkMargin;

			//For every line to display, calculate height required, generate new rectangle based on it, then draw.
			//We'll start with the basic text. ("You are awarded...")
			uiText->print("You are awarded...", *testRect, DT_CALCRECT | DT_WORDBREAK);
			actualRect->bottom = actualRect->top + testRect->bottom;
			uiText->print("You are awarded...", *actualRect, DT_CENTER | DT_WORDBREAK);
			//Reset actaulRect to work for the next text
			actualRect->top = actualRect->bottom + uiNS::talkMargin;
			testRect->right = widthGiven;

			vector<ConditionalSkillReward>* skillsRewards = questToDisplay->getSkillsRewards();
			stringstream ss;
			//Now show skills reward
			for (vector<ConditionalSkillReward>::iterator i = skillsRewards->begin(); i != skillsRewards->end(); i++)
			{
				if (i->name == "" || ((Grpg*)theGame)->getQuestLoader()->getQuestData()->getValue(i->name) == i->requiredValue)
				{
					string skillName = player->getSkills()->at(i->skillToIncrease).getSkill()->getName();
					int amountOfXP = i->XPtoGain;
					ss << "+" << amountOfXP << " " << skillName << " XP";
					uiText->print(ss.str().c_str(), *testRect, DT_CALCRECT | DT_WORDBREAK);
					actualRect->bottom = actualRect->top + testRect->bottom;
					uiText->print(ss.str().c_str(), *actualRect, DT_CENTER | DT_WORDBREAK);
					//Reset actaulRect to work for the next text
					actualRect->top = actualRect->bottom + uiNS::talkMargin;
					testRect->right = widthGiven;
					//Reset string to contian nothing
					ss.str("");
				}
			}
			//Now show items reward
			vector<InventoryItem*> itemsReward = questToDisplay->getItemRewards();
			for (vector<InventoryItem*>::iterator i = itemsReward.begin(); i != itemsReward.end(); i++)
			{
				InventoryItem* theItem = *i;
				ss << theItem->getCurrentStackCount() << " x " << theItem->getName();
				uiText->print(ss.str().c_str(), *testRect, DT_CALCRECT | DT_WORDBREAK);
				actualRect->bottom = actualRect->top + testRect->bottom;
				uiText->print(ss.str().c_str(), *actualRect, DT_CENTER | DT_WORDBREAK);
				//Reset actaulRect to work for the next text
				actualRect->top = actualRect->bottom + uiNS::talkMargin;
				testRect->right = widthGiven;
				ss.str("");
			}
			//Now show text reward
			vector<string> miscReward = questToDisplay->getMiscRewards();
			for (vector<string>::iterator i = miscReward.begin(); i != miscReward.end(); i++)
			{
				uiText->print(*i, *testRect, DT_CALCRECT | DT_WORDBREAK);
				actualRect->bottom = actualRect->top + testRect->bottom;
				uiText->print(*i, *actualRect, DT_CENTER | DT_WORDBREAK);
				//Reset actaulRect to work for the next text
				actualRect->top = actualRect->bottom + uiNS::talkMargin;
				testRect->right = widthGiven;
			}
			delete testRect;
			delete actualRect;
		}
		//Display conversation, if any
		else if (chatText.size() > 0)
		{
			//Find out how much of the window we can work with
			//One talk margin will be applied once at the start for some space
			float heightGiven = windowImage.getHeight() - uiNS::windowXHeight - uiNS::windowBottomBorder - uiNS::talkMargin;
			float widthGiven = windowImage.getWidth() - uiNS::windowLRMargin - uiNS::windowLRMargin;

			//Marks if the remaining text will no longer fit, no need to display any more
			//-1: Not yet, everything's good.
			//A number: the chatText in this number, and everything before it, will no longer fit
			int abortNo = -1;

			for (int i = chatText.size() - 1; i >= 0; i--)
			{
				if (abortNo != -1)
				{
					delete chatText[i];
				}
				else
				{
					heightGiven -= chatText[i]->getHeightTaken();
					heightGiven -= uiNS::talkMargin;

					if (heightGiven < 0)
					{
						//Not going to work out, abort this line and everything after it
						delete chatText[i];
						abortNo = i;
					}
				}
			}
			//Remove all deleted items, if any
			if (abortNo != -1)
			{
				chatText.erase(chatText.begin(), chatText.begin() + abortNo + 1);
			}

			//Now begin the drawing
			float startLeft = windowImage.getX() - windowImage.getWidth() / 2 + uiNS::windowLRMargin;
			float startTop = windowImage.getY() - windowImage.getHeight() / 2 + uiNS::windowXHeight + uiNS::talkMargin;
			float startRight = windowImage.getX() + windowImage.getWidth() / 2 - uiNS::windowLRMargin;

			RECT* textRect = new RECT();

			for (int i = 0; i < chatText.size(); i++)
			{
				textRect->top = startTop;
				textRect->bottom = startTop + chatText[i]->getHeightTaken();
				textRect->left = startLeft;
				textRect->right = startRight;

				chatText[i]->draw(textRect, uiText);

				startTop += chatText[i]->getHeightTaken() + uiNS::talkMargin;
			}
			delete textRect;
		}
	}

	if (showMainMenu)
	{
		mainMenuImage.draw();
	}

	if (newChatVersion && messageDisplayTime > 0)
	{
		newTextBackground.draw();
		uiText->print(message, *newTextRect, DT_CENTER);
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

	Image* relevantImage = tabImages[tabNumber - 1];
	relevantImage->setX(tabImage.getX());
	relevantImage->setY(tabImage.getY());
	relevantImage->draw();
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

	if (tabNumber == uiNS::OPTIONS)
	{
		uiText->print("Options", topLeftX + 5, topLeftY + 5);
		//Draw show health bar options
		topLeftY += 25;
		Image* imageToShow;
		if (showHealth)
		{
			imageToShow = &checkboxSelectedImage;
		}
		else
		{
			imageToShow = &checkboxImage;
		}
		imageToShow->setX(topLeftX + uiNS::optionWidth/2 + 5);
		imageToShow->setY(topLeftY + uiNS::optionHeight / 2);
		imageToShow-> draw();
		uiText->print("Show bottom\nHealthbar", topLeftX + uiNS::optionWidth + 10, topLeftY);

		if (newChatVersion)
		{
			topLeftY += 12 + uiNS::optionHeight;
			if (showChatHistory)
			{
				imageToShow = &checkboxSelectedImage;
			}
			else
			{
				imageToShow = &checkboxImage;
			}
			imageToShow->setX(topLeftX + uiNS::optionWidth / 2 + 5);
			imageToShow->setY(topLeftY + uiNS::optionHeight / 2);
			imageToShow->draw();
			uiText->print("Show Chat History", topLeftX + uiNS::optionWidth + 10, topLeftY);
		}
	}
	else if (tabNumber == uiNS::SKILLS)
	{
		float heightAllowed = uiNS::HEIGHT / uiNS::skillsPerColumn;
		float skillsWidth = getX() - topLeftX;
		map<int, PlayerSkill>* playerSkills = player->getSkills();
		map<int, PlayerSkill>::iterator it;
		stringstream skillLevel;
		int counter = 0;
		for (it = playerSkills->begin(); it != playerSkills->end(); it++)
		{
			//Print the skill text at the center of each location, with 5 px margin: left;
			skillsText->print(it->second.getSkill()->getName(),
				topLeftX + 5, topLeftY);
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
			//Draw it at the right, aligned bottom
			// Calculate the text side
			RECT* textRect = new RECT();
			textRect->left = topLeftX + 5;
			textRect->top = topLeftY;

			textRect->right = textRect->left + skillsWidth  - 20; //20 for margin cause the skills background at the right
																	//does have some space that should not be printed on
			textRect->bottom = topLeftY + heightAllowed;

			//Align level bottom right
			skillsText->print(skillLevel.str() + "/99", *textRect, DT_BOTTOM | DT_RIGHT | DT_SINGLELINE);

			delete textRect;

			skillLevel.str("");

			topLeftY += heightAllowed;
			
			counter++;
			if (counter == 4) //End of combat skills
			{
				topLeftY = getTopLeftY();// getY() - uiNS::HEIGHT / 2;
				topLeftX = getX();
			}
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
			//Make sure items held by the player always displays no matter which realm
			//the player is in
			it->second->setIsInDarkRealm(thePlayer->inDarkRealm());
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
		graphics->spriteEnd();
		graphics->spriteBegin();
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
	player->sayMessage(commandStr);
	return false;								// return command
}

//=============================================================================
// Add text to console
// Only the first line of text in str will be displayed.
//=============================================================================
void UI::addChatText(const std::string &str)     // add text to console
{
	if (messageShown)
	{
		messageShown = false;
		message = "";
	}
	stringstream ss;
	string line = "";
	char tempChar;
	ss << str;
	tempChar = ss.get();
	while (tempChar != EOF){
		if (tempChar == '\n'){
			text.push_front(line);
			if (newChatVersion)
			{
				message += line + "\n";
			}
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
	if (newChatVersion)
	{
		message += line + "\n";
	}
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void UI::update(float frameTime, Game* gamePtr)
{
	if (messageDisplayTime > 0)
	{
		messageDisplayTime -= frameTime;
	}

	if (!messageShown)
	{
		messageShown = true;
		newTextRect->left = 0;
		newTextRect->top = 0;
		uiText->print(message, *newTextRect, DT_CALCRECT);
		int width = newTextRect->right;
		int height = newTextRect->bottom;
		newTextRect->bottom = GAME_HEIGHT - uiNS::healthHeight;
		newTextRect->top = newTextRect->bottom - height;
		newTextRect->left = GAME_WIDTH / 2 - width / 2;
		newTextRect->right = newTextRect->left + width;
		if (!newTextBackground.initialize(graphics, newTextRect->left, newTextRect->top, width, height, graphicsNS::BLACK, ""))
			throw new GameError(gameErrorNS::FATAL_ERROR, "Error initalizing text message black background");
		messageDisplayTime = uiNS::initialMessageDisplayTime;
		//Increase time based on how long the message is
		for (size_t i = 0; i < message.length(); i++)
		if (message[i] == '\n')
			messageDisplayTime += uiNS::initialMessageDisplayTime;
	}

	if (chatTimer != -1)
	{
		chatTimer -= frameTime;
		while (chatTimer <= 0 && chatTimer != -1)
		{
			if (preChatText.size() == 0)
			{
				chatTimer = -1;
			}
			else
			{
				chatText.push_back(preChatText[0]);
				preChatText.erase(preChatText.begin(), preChatText.begin()+1);
				chatTimer += uiNS::talkDelay;
			}
		}
	}	

	if (lastLevelUpSkill != nullptr)
	{
		lastLevelUpSkill->update(frameTime, gamePtr);
	}
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
			removeWindow();
			return true;
		}
		//Check for selection in options
		for (int i = 0; i < chatText.size(); i++)
		{
			if (chatText[i]->getType() == chatNS::DECISIONTYPE)
			{
				bool result = ((ChatDecision*)chatText[i])->checkMouseClick(input->getMouseX(), input->getMouseY(), this);
				if (result)
				{
					return true;
				}
			}
		}
	}

	if (activeTab == uiNS::QUESTS)
	{
		map<int, Quest*>* mapQuests = ((Grpg*)theGame)->getQuestLoader()->getMapQuests();
		for (std::map<int, Quest*>::iterator it = mapQuests->begin(); it != mapQuests->end(); ++it)
		{//draw quests
			if (it->second->getUIElement()->mouseOver(input->getMouseX(), input->getMouseY()))
			{
				removeWindow();
				//this->addChatText("Clicked quest:" + it->second->getname());
				drawWindow("Quest: " + it->second->getname());
				((Grpg*)theGame)->getQuestLoader()->setCurrentlyShownIndex(it->first);
				setActiveTab(uiNS::QUESTS);//fixed
				it->second->printHelpTextToUI(this);
				return true;
			}
		}
	}
	else if (activeTab == uiNS::OPTIONS)
	{
		if (input->getMouseX() > getTopLeftX() && input->getMouseX() < getTopLeftX() + uiNS::WIDTH)
		{
			//Show health option
			if (input->getMouseY() > getTopLeftY() + 25 && input->getMouseY() < getTopLeftY() + 25 + uiNS::optionHeight)
			{
				showHealth = !showHealth;
			}
			//show chat history option
			if (newChatVersion)
			{
				if (input->getMouseY() > getTopLeftY() + 25 + uiNS::optionHeight + 12 && input->getMouseY() < getTopLeftY() + 25 + uiNS::optionHeight + 12 + uiNS::optionHeight)
				{
					showChatHistory = !showChatHistory;
				}
			}
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
		SAFE_DELETE(theItem->teleportBehavior);
		SAFE_DELETE(theItem->gainXPBehavior);
		if (theItem->buyBehavior == nullptr)
		{
			theItem->buyBehavior = new BuyBehavior(player, theItem, coin, (Grpg*)game);
		}
		theItem->setupVectorActiveBehaviors();
	}
	items = i;

	//Make the player's active tab the inventory
	activeTab = uiNS::INVENTORY;
}

void UI::displayQuestReward(Quest* quest)
{
	questToDisplay = quest;
	drawWindow(quest->getname() + " complete!");
}

void UI::newSkillLevelUP(string skillName)
{
	SkillLevelUp* slu = new SkillLevelUp(levelUpImage, lastLevelUpSkill, skillName, levelUpFont, this);
	if (lastLevelUpSkill != nullptr)
	{
		lastLevelUpSkill->setNextLevelUp(slu);
	}
	lastLevelUpSkill = slu;
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
	newTextBackground.onLostDevice();
	rightClickBackground.onLostDevice();
	darkRealmVision.onLostDevice();
	levelUpFont->onLostDevice();
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
	newTextBackground.onResetDevice();
	rightClickBackground.onResetDevice();
	darkRealmVision.onResetDevice();
	levelUpFont->onResetDevice();
}
