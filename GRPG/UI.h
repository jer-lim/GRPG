//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H

#ifndef _UI_H                 // Prevent multiple definitions if this 
#define _UI_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Button.h"
#include "Behavior.h"
#include "BuyBehavior.h"
#include "SellBehavior.h"
#include "ChatData.h"

class Quest;

namespace uiNS
{
	const char mainMenuLocation[] = "assets/ui/main_menu3.png";

	const int WIDTH = 190;                   // image width
	const int HEIGHT = 264;                  // image height
	const int X = GAME_WIDTH - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT - HEIGHT / 2;
	const int textSize = 20;
	const int skillsSize = 17;

	const int OPTIONS = 1;
	const int SKILLS = 2;
	const int INVENTORY = 3;
	const int EQUIPS = 4;
	const int QUESTS = 5;
	const int totalTabs = 5;
	const char optionsImage[] = "assets/ui/options.png";
	const char skillsImage[] = "assets/ui/skills.png";
	const char inventoryImage[] = "assets/ui/inventory.png";
	const char equipmentImage[] = "assets/ui/equipment.png";
	const char questsImage[] = "assets/ui/quests.png";
	const char checkboxImage[] = "assets/ui/checkbox.png";
	const char checkboxSelectedImage[] = "assets/ui/checkbox_selected.png";
	const char chatHistoryImage[] = "assets/ui/chatHistory.png";
	const float chatHistoryLeftMargin = 94;
	const float chatHistoryBottomMargin = 127;
	const float chatHistoryWidth = 383;

	const int optionHeight = 25;
	const int optionWidth = 25;

	const int tabWIDTH = 34;
	const int tabHEIGHT = 40;
	const int tabLMargin = 7;
	const int tabMargin = 2;

	const UINT chatHeight = 200;
	const UINT chatWidth = 500;
	const float initialMessageDisplayTime = 2;
	const COLOR_ARGB chatColour = SETCOLOR_ARGB(50, 0, 0, 0);

	const COLOR_ARGB darkRealmColour = SETCOLOR_ARGB(66, 0, 0, 0);

	const UINT healthHeight = 40;
	// Calculate the width of the health bar
	// Health bar centralizes in the middle, left maximum chat, right as much as left
	const float healthWidth = (GAME_WIDTH / 2 - uiNS::chatWidth) * 2;
	const COLOR_ARGB healthColor = SETCOLOR_ARGB(180, 255, 0, 0);
	const COLOR_ARGB noHealthColor = SETCOLOR_ARGB(180, 0, 0, 0);

	const COLOR_ARGB rightClickBG = SETCOLOR_ARGB(255, 0, 0, 0);

	// Coordindates referring to the height and width of the X icon at the top left
	// of the window texture
	const UINT windowXWidth = 28;
	const UINT windowXHeight = 17;
	//The border of the window
	const int windowLeftBorder = 6;
	const int windowBottomBorder = 7;
	//The margin given for chat (applies on both left and right)
	const int windowLRMargin = 15;
	//The margin applied between talking text
	const int talkMargin = 10;
	// The delay between which each chat appears
	const float talkDelay = 1;

	//Coordindates for shop
	const int shopColMax = 12;
	//Margins from the edges of the image
	const int shopLMargin = 20;
	const int shopTMargin = 41;
	// Margins between shop items
	const int shopWMargin = 6;
	const int shopHMargin = 15;

	const int skillsPerColumn = 7;
}

// inherits from Entity class
class UI : public Entity
{
private:

	//GENERAL UI VARIABLES
	Game* game;
	TextDX* uiText;
	TextDX* skillsText;
	Player* player; //Pointer to player to retrieve information about them
	Input* input; //The input system

	// TABBING SYSTEM

	//The images that will draw the tab background onto the screen
	Image tabImage;
	TextureManager* tabTexture; // The texture of the tabs
	TextureManager* uiImgTexture;
	//The current active tab. From uiNS, either OPTIONS, SKILLS, INVENTORY, EQUIPMENT or QUESTS
	int activeTab;
	//A vector of UI tabbing images
	vector<Image*> tabImages;
	vector<TextureManager*> tabTextures;
	//For options tab
	Image checkboxImage;
	TextureManager* checkboxTexture;
	Image checkboxSelectedImage;
	TextureManager* checkboxSelectedTexture;

	// CHAT VARIABLES

	Button chatRect;				// Vertex data to show the chat rectangle at the bottom left
	RECT textRect;               // text rectangle, used to specify and draw the chat box
	std::deque<std::string> text;       // UI text
	int rows;	//Total number of rows in the chat
	int rowHeight;	//Amount of pixels each height take in the chat box

	//Display last known message at bottom right of the screen
	string message;
	//Whether the message has been shown. If yes, then will overwrite message on next message request.
	bool messageShown;
	bool newChatVersion;
	RECT* newTextRect;
	Button newTextBackground;
	float messageDisplayTime;
	bool showChatHistory;
	Image chatHistoryImage;
	TextureManager* chatHistoryTexture;

	// HEALTH BAR
	Button health;				//Vertex data for the entirety of the health bar
	Button availableHealth;		// Vertex data for only the health that the player has
	bool showHealth;

	// Right click menu
	vector<Behavior*> menus;
	VECTOR2 menuTop;
	Button rightClickBackground;	// Vertex data for drawing the background

	// WINDOW SYSTEM

	// The background of the big window that appears in the middle of the game
	//Used for store, quests etc.
	TextureManager* windowTexture;
	Image windowImage; // The window image that is drawn in the middle
	// The header text in the window. If empty, signifies no window and no window will be drawn
	string windowHeader;
	RECT* shopRect; // The rectangle used for drawing the shop text
	vector<Entity* > items; //The list of items that will be sold in the shop
	Entity* coin;		//Track player's coin and perform changes
	TextureManager* shopTexture;
	Image shopImage;
	// The list of chat text displayed in the window during chat
	vector<ChatData*> chatText; 
	// The list of chat text that is about to be displayed in the chat window
	vector<ChatData*> preChatText;
	//Starts at uiNS::talkDelay, ticks down to 0, then displays next text. If -1, no text in queue
	float chatTimer;
	//The quest's reward to display in the window, if any
	Quest* questToDisplay;

	//Dark realm vision
	Button darkRealmVision;

	//Main menu
	Image mainMenuImage;
	TextureManager* mainMenuTexture;
	bool showMainMenu;
protected:
	//Draws the specified tab number onto the screen on the correct location
	//Also see drawTabContents
	void drawTab(int tabNumber);

	//Draws the specified tab contents onto the screen on the correct location
	//Also see drawTab
	void drawTabContents(int tabNumber);
public:
	// constructor
	UI();

	// Destructor
	~UI();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Input *in);

	// Process console command
	// Returns true if processing was done, false otherwise
	virtual bool UI::processCommand(const std::string commandStr);

	// Checks if the mouse is currently over any part of the UI. Returns true if so
	virtual bool mouseInside(Viewport* vp);

	// Add text to console
	// Only the first line of text in str will be displayed.
	virtual void addChatText(const std::string &str);     // add text to console

	//Draws a right click 
	//See removeRightClickMenu
	virtual void setRightClickMenu(vector<Behavior*> behaviors);

	//Stop drawing the right click menu
	//See setRightClickMenu
	virtual void removeRightClickMenu() { rightClickBackground.setVisible(false); }

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	void update(float frameTime, Game* gamePtr);

	// Takes note that a click was performed and processes it
	// Checking which part of the UI it was over and performing the action
	// Returns true if something was processed in the UI
	// Returns false if nothing was processed
	bool UI::performClick();

	float getTopLeftX(){ return getX() - uiNS::WIDTH / 2; }
	float getTopLeftY(){ return getY() - uiNS::HEIGHT / 2; }

	virtual string getType(){ return "UI"; }

	// Start drawing the window in the middle of the screen, with the specified header
	// that is shown at the top of the window
	// Warning: You MUST specify a header, or the window won't be drawn
	virtual void drawWindow(string header)
	{
		windowHeader = header;
	}

	//Adds a new talking text for during a chatting window
	//The drawWindow function MUST have been called previously and up, otherwise nothing will be shown
	//Remember that if the player moves, the window is automatically set to disappear, so take note of that
	//You may want to do stuff like reset the conversation
	//ChatData can be either a ChatInformation for talking or a ChatDecision for a decision required by the player
	virtual void addTalkText(ChatData* text)
	{
		if (text->getType() == chatNS::DECISIONTYPE)
		{
			((ChatDecision*)text)->setGraphics(graphics);
		}
		text->calculateHeightTaken(uiText, windowImage.getWidth() - uiNS::windowLRMargin - uiNS::windowLRMargin);
		preChatText.push_back(text);
		if (chatTimer == -1)
		{
			chatTimer = 0;
		}
	}

	// Removes the shop window, if any, is being drawn on the screen
	// Also removes all sell behavior from items in the shop, if required
	// As well as clearing the chat data
	// Also removes the quest's reward that is currently being displayed, if any
	virtual void removeWindow()
	{
		windowHeader = "";
		questToDisplay = nullptr;
		items.clear();
		//Remove sell behavior if required
		vector<Entity*> playerInventory = player->getInventory()->getVectorItems();
		for (vector<Entity*>::iterator it = playerInventory.begin(); it != playerInventory.end(); ++it)
		{
			Entity* theItem = *it;
			SAFE_DELETE(theItem->sellBehavior);
			theItem->setupVectorActiveBehaviors();
		}
		removeAllChatData();
	}

	//Begins displaying the quest reward of a certain quest, as if the player has just completed it.
	//Note that it only displays the quest, it doesn't give the reward or anything like that
	//You do NOT need to call 'drawWindow()' for this
	//To remove, call removeWindow()
	virtual void displayQuestReward(Quest* quest);

	//Deletes and removes all stored chat data in the UI, if any.
	virtual void removeAllChatData()
	{
		//Remove text in chat if required
		for (int i = 0; i < chatText.size(); i++)
		{
			delete chatText[i];
		}
		chatText.clear();
		for (int i = 0; i < preChatText.size(); i++)
		{
			delete preChatText[i];
		}
		preChatText.clear();
	}

	virtual void setShopItems(vector<Entity* > i);

	//Gets the items shown in the shop. If empty, no items are shown
	virtual vector<Entity*> getShopItems() { return items; }

	// Gets the string shown in the window header
	// If string is an empty string ("") no window is currently open
	virtual string getWindowHeader() { return windowHeader; }

	int getActiveTab(){ return activeTab; }
	void setActiveTab(int i)
	{
		activeTab = i;
	}

	bool getShowMainMenu() { return showMainMenu; }
	void setShowMainMenu(bool s) { showMainMenu = s; }
};
#endif